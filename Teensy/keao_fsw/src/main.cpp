#include <vector>
#include <Arduino.h>
#include <USBHost_t36.h>
#include <support/configCosmosKernel.h>
#include <pdu.h>
#include "channels/artemis_channels.h"
#include "artemis_devices.h"
#include "artemisbeacons.h"
#include "tests/packet_tests.h"

// For setting Teensy Clock Frequency (only for Teensy 4.0 and 4.1)
#if defined(__IMXRT1062__)
extern "C" uint32_t set_arm_clock(uint32_t frequency);
#endif

void send_beacons(void);
void req_switch_beacon(void);
bool turn_on_pi(void);

namespace
{
  using namespace Artemis::Teensy;
  Artemis::Devices devices;
  PacketComm packet;
  USBHost usb;
  elapsedMillis sensortimer;
  elapsedMillis uptime;
  int32_t iretn = 0;
}

void setup()
{
  Serial.begin(115200);

#if defined(__IMXRT1062__)
  set_arm_clock(450000000); // Allowed Frequencies (MHz): 24, 150, 396, 450, 528, 600
#endif

  usb.begin();
  pinMode(RPI_ENABLE, OUTPUT);
  pinMode(UART6_RX, INPUT);
  delay(3000);

  // Set up devices
  iretn = devices.setup_magnetometer();
  if (iretn < 0)
  {
    Serial.println("Fail to initialize magnetometer");
  }
  iretn = devices.setup_imu();
  if (iretn < 0)
  {
    Serial.println("Fail to initialize IMU");
  }
  devices.setup_current();
  devices.setup_gps();

  threads.setSliceMillis(10);

  // Add Channels / Threads
  thread_list.push_back({threads.addThread(Channels::rfm23_channel, 9000), Channels::Channel_ID::RFM23_CHANNEL});
  thread_list.push_back({threads.addThread(Channels::rfm98_channel, 9000), Channels::Channel_ID::RFM98_CHANNEL});
  thread_list.push_back({threads.addThread(Channels::pdu_channel, 9000), Channels::Channel_ID::PDU_CHANNEL});
  thread_list.push_back({threads.addThread(Channels::astrodev_channel, 9000), Channels::Channel_ID::ASTRODEV_CHANNEL});
  thread_list.push_back({threads.addThread(Channels::rpi_channel, 9000), Channels::Channel_ID::RPI_CHANNEL});

  Serial.println("Setup Complete");
}

void loop()
{
  // Test packets. Enable/disable in platformio.ini with the '-D TEST_PACKETS' flag.
#ifdef TEST_PACKETS
  send_test_packets();
  threads.delay(5000);
#endif

  // Pull any packets sent to the main thread
  if (PullQueue(packet, main_queue, main_queue_mtx))
  {
    // Packet meant to be downlink to ground station
    if (packet.header.nodedest == (uint8_t)NODES::GROUND_NODE_ID)
    {
      // Select which radio to downlink on
      switch (packet.header.chanout)
      {
      case Channels::Channel_ID::RFM23_CHANNEL:
      {
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        break;
      }
      case Channels::Channel_ID::ASTRODEV_CHANNEL:
      {
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
        break;
      }
      default:
        break;
      }
    }

    // Packet meant to be forwarded to Raspberry Pi
    else if (packet.header.nodedest == (uint8_t)NODES::RPI_NODE_ID)
    {
      // If Raspberry Pi is not on
      if (!digitalRead(UART6_RX))
      {
        // Attempt to turn on Pi. If unsuccessful, downlink switch beacon
        if (!turn_on_pi())
        {
          req_switch_beacon();
        }
      }
      // Push packet to RPI channel
      PushQueue(packet, rpi_queue, rpi_queue_mtx);
    }

    // Packet is meant for myself (Teensy)
    else if (packet.header.nodedest == (uint8_t)NODES::TEENSY_NODE_ID)
    {
      // Handle packet types
      switch (packet.header.type)
      {

      // If received a Ping packet, reply with a Pong packet
      case PacketComm::TypeId::CommandObcPing:
      {
        packet.header.nodedest = packet.header.nodeorig;
        packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
        packet.header.type = PacketComm::TypeId::DataObcPong;
        packet.data.resize(0);
        const char *data = "Pong";
        for (size_t i = 0; i < strlen(data); i++)
        {
          packet.data.push_back(data[i]);
        }

        // Downlink on requested radio
        switch (packet.header.chanout)
        {
        case Channels::Channel_ID::RFM23_CHANNEL:
        {
          PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
          break;
        }
        case Channels::Channel_ID::ASTRODEV_CHANNEL:
        {
          PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
          break;
        }
        default:
          break;
        }
        break;
      }
      // If received an EPS Communicate packet, forward to PDU
      case PacketComm::TypeId::CommandEpsCommunicate:
      {
        PushQueue(packet, pdu_queue, pdu_queue_mtx);
        break;
      }
      // If received a Switch Control packet (enable/disable a switch)
      case PacketComm::TypeId::CommandEpsSwitchName:
      {
        PDU::PDU_SW switchid = (PDU::PDU_SW)packet.data[0];
        switch (switchid)
        {
        // Controlling RPI Switch. Do not forward to PDU
        case PDU::PDU_SW::RPI:
        {
          if (packet.data[1] == 1 && !turn_on_pi())
          {
            req_switch_beacon();
          }
          else if (packet.data[1] == 0)
          {
            PushQueue(packet, rpi_queue, rpi_queue_mtx);
          }
          break;
        }
        // Any other switch, forward to be handled by PDU
        default:
          PushQueue(packet, pdu_queue, pdu_queue_mtx);
          break;
        }
        break;
      }
      // If a Switch Status Request packet
      case PacketComm::TypeId::CommandEpsSwitchStatus:
      {
        PDU::PDU_SW switchid = (PDU::PDU_SW)packet.data[0];
        switch (switchid)
        {
        // Read digital pin of RPI Switch. Not handled by PDU
        case PDU::PDU_SW::RPI:
        {
          packet.data.resize(1);
          packet.data.push_back(digitalRead(RPI_ENABLE));
          packet.header.type = PacketComm::TypeId::DataEpsResponse;
          packet.header.nodedest = packet.header.nodeorig;
          packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;

          // Downlink on requested radio
          switch (packet.header.chanout)
          {
          case Channels::Channel_ID::RFM23_CHANNEL:
          {
            PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
            break;
          }
          case Channels::Channel_ID::ASTRODEV_CHANNEL:
          {
            PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
            break;
          }
          default:
            break;
          }
          break;
        }
        // Otherwise, forward to be handled by PDU
        default:
          PushQueue(packet, pdu_queue, pdu_queue_mtx);
          break;
        }
        break;
      }
      // If a Beacon Request Packet
      case PacketComm::TypeId::CommandObcSendBeacon:
      {
        send_beacons();
        break;
      }
      default:
        break;
      }
    }
  }

  // Periodic Telemetry
  if (sensortimer > 10000)
  {
    sensortimer = 0;
    send_beacons();
  }
  devices.update_gps();

  threads.delay(10);
}

/*********************************************************
 *****               HELPER FUNCTIONS                *****
 *********************************************************/

void send_beacons(void)
{
  devices.read_temperature(uptime);
  devices.read_current(uptime);
  devices.read_imu(uptime);
  devices.read_mag(uptime);
  devices.read_gps(uptime);

  req_switch_beacon();
}

void req_switch_beacon(void)
{
  // Get PDU Switches
  packet.header.type = PacketComm::TypeId::CommandEpsSwitchStatus;
  packet.header.nodeorig = (uint8_t)NODES::GROUND_NODE_ID;
  packet.header.nodedest = (uint8_t)NODES::TEENSY_NODE_ID;
  packet.data.clear();
  packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::All);
  PushQueue(packet, pdu_queue, pdu_queue_mtx);
}

bool turn_on_pi(void)
{
  // Read voltage of battery (V_BATT)
  float curr_V = devices.current_sensors["battery_board"]->getBusVoltage_V();
  Serial.println(curr_V);
  // If battery is at least 7.0V, turn on Pi
  if (curr_V >= 7.0)
  {
    digitalWrite(RPI_ENABLE, HIGH);
    thread_list.push_back({threads.addThread(Channels::rpi_channel), Channels::Channel_ID::RPI_CHANNEL});
    return true;
  }

  return false;
}
