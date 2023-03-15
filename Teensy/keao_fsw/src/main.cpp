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

  // Threads
  // thread_list.push_back({threads.addThread(Channels::rfm23_channel, 9000), Channels::Channel_ID::RFM23_CHANNEL});
  // thread_list.push_back({threads.addThread(Channels::rfm98_channel, 9000), Channels::Channel_ID::RFM98_CHANNEL});
  thread_list.push_back({threads.addThread(Channels::pdu_channel, 9000), Channels::Channel_ID::PDU_CHANNEL});
  // thread_list.push_back({threads.addThread(Channels::astrodev_channel, 9000), Channels::Channel_ID::ASTRODEV_CHANNEL});

  Serial.println("Setup Complete");
}

void loop()
{
#ifdef TEST_PACKETS
  send_test_packets();
  threads.delay(5000);
#endif

  if (PullQueue(packet, main_queue, main_queue_mtx))
  {
    if (packet.header.nodedest == (uint8_t)NODES::GROUND_NODE_ID)
    {
      switch (packet.header.chanout)
      {
      case Channels::Channel_ID::RFM23_CHANNEL:
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        break;
      case Channels::Channel_ID::ASTRODEV_CHANNEL:
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
        break;
      default:
        break;
      }
    }
    else if (packet.header.nodedest == (uint8_t)NODES::RPI_NODE_ID)
    {
      PushQueue(packet, rpi_queue, rpi_queue_mtx);
    }
    else if (packet.header.nodedest == (uint8_t)NODES::TEENSY_NODE_ID)
    {
      switch (packet.header.type)
      {
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
        switch (packet.header.chanout)
        {
        case Channels::Channel_ID::RFM23_CHANNEL:
          PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
          break;
        case Channels::Channel_ID::ASTRODEV_CHANNEL:
          PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
          break;
        default:
          break;
        }
        break;
      }
      case PacketComm::TypeId::CommandEpsCommunicate:
      {
        PushQueue(packet, pdu_queue, pdu_queue_mtx);
      }
      break;
      case PacketComm::TypeId::CommandEpsSwitchName:
      {
        PDU::PDU_SW switchid = (PDU::PDU_SW)packet.data[0];
        switch (switchid)
        {
        case PDU::PDU_SW::RPI:
        {

          float curr_V = devices.current_sensors["battery_board"]->getBusVoltage_V();
          Serial.println(curr_V);
          if ((packet.data[1] == 1 && curr_V >= 7.0) || (packet.data[1] == 1 && packet.data[2] == 1))
          {
            Serial.println(packet.data[1]);
            digitalWrite(RPI_ENABLE, packet.data[1]);
            thread_list.push_back({threads.addThread(Channels::rpi_channel), Channels::Channel_ID::RPI_CHANNEL});
          }
          else if (packet.data[1] == 0)
          {
            PushQueue(packet, rpi_queue, rpi_queue_mtx);
          }
          break;
        }
        default:
          PushQueue(packet, pdu_queue, pdu_queue_mtx);
          break;
        }
        break;
      }
      case PacketComm::TypeId::CommandEpsSwitchStatus:
      {
        PDU::PDU_SW switchid = (PDU::PDU_SW)packet.data[0];
        switch (switchid)
        {
        case PDU::PDU_SW::RPI:
        {
          packet.data.resize(1);
          packet.data.push_back(digitalRead(RPI_ENABLE));
          packet.header.type = PacketComm::TypeId::DataEpsResponse;
          packet.header.nodedest = packet.header.nodeorig;
          packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
          PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        }
        break;
        default:
          PushQueue(packet, pdu_queue, pdu_queue_mtx);
          break;
        }
      }
      break;
      case PacketComm::TypeId::CommandObcSendBeacon:
      {
        devices.read_temperature(uptime);
        devices.read_current(uptime);
        devices.read_imu(uptime);
        devices.read_mag(uptime);
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
    devices.read_temperature(uptime);
    devices.read_current(uptime);
    devices.read_imu(uptime);
    devices.read_mag(uptime);
    devices.read_gps(uptime);
  }
  devices.update_gps();

  threads.delay(10);
}
