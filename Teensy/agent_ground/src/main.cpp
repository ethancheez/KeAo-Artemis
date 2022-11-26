#include <Arduino.h>
#include <NativeEthernet.h>
#include <vector>
#include <map>
#include <string.h>
#include <artemis_channels.h>
#include <support/configCosmosKernel.h>
#include "artemisbeacons.h"

void handle_cmd();

namespace
{
  char buf[256];
  PacketComm packet;

  // Ethernet
  byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
  IPAddress remoteIp(192, 168, 150, 197); // Edit this to the IP of your computer runing COSMOS Web
  unsigned short remotePort = 65535;
  unsigned short localPort = 65535;
  EthernetUDP udp;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    continue;

  Serial.println("Initializing Ethernet...");
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Ethernet not connected");
  }
  udp.begin(localPort);

  threads.setSliceMillis(10);

  // Threads
  thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm23_channel), "rfm23 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm98_channel), "rfm98 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::astrodev_channel), "astrodev thread"});
}

void loop()
{
  handle_cmd();

  if (PullQueue(packet, main_queue, main_queue_mtx))
  {
    StaticJsonDocument<1100> doc;
    doc["node_name"] = "artemis_teensy";

    switch (packet.header.type)
    {
    case PacketComm::TypeId::DataBeacon:
    {
      switch ((TypeId)packet.data[0])
      {
      case TypeId::current1:
      {
        Serial.println("Recieved Current Beacon 1");
        currentbeacon1 beacon1;
        memcpy(&beacon1, packet.data.data(), sizeof(beacon1));
        doc["time"] = beacon1.deci;
        for (int i = 0; i < ARTEMIS_CURRENT_BEACON_1_COUNT; i++)
        {
          JsonObject current1 = doc.createNestedObject(current_sen_names[i]);
          current1["vol"] = beacon1.busvoltage[i];
          current1["cur"] = beacon1.current[i];
        }
        break;
      }
      case TypeId::current2:
      {
        Serial.println("Recieved Current Beacon 2");
        currentbeacon2 beacon2;
        memcpy(&beacon2, packet.data.data(), sizeof(beacon2));
        doc["time"] = beacon2.deci;
        Serial.println(beacon2.deci);
        break;
      }
      case TypeId::temperature:
      {
        Serial.println("Recieved Temperature Beacon");
        temperaturebeacon beacon;
        memcpy(&beacon, packet.data.data(), sizeof(beacon));
        doc["time"] = beacon.deci;
        Serial.println(beacon.deci);
        break;
      }
      case TypeId::imu:
      {
        Serial.println("Recieved IMU Beacon 1");
        imubeacon beacon;
        memcpy(&beacon, packet.data.data(), sizeof(beacon));
        doc["time"] = beacon.deci;
        Serial.println(beacon.deci);
        break;
      }
      case TypeId::mag:
      {
        Serial.println("Recieved IMU Beacon 2");
        magbeacon beacon;
        memcpy(&beacon, packet.data.data(), sizeof(beacon));
        doc["time"] = beacon.deci;
        Serial.println(beacon.deci);
        break;
      }
      default:
        break;
      }
      // Send UDP packet
      udp.beginPacket(remoteIp, remotePort);
      serializeJson(doc, udp);
      udp.println();
      udp.endPacket();
      break;
    }
    default:
      break;
    }
  }
}

void handle_cmd()
{
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    char *cmdline = input.begin();

    vector<string> args;

    for (char *token = strtok(cmdline, " "); token != NULL; token = strtok(NULL, " "))
    {
      args.push_back(string(token));
    }

    if (args.size() < 3)
    {
      Serial.println("COMMAND SYNTAX: <node_dest> <radio_out>:<radio_in> <command> [args]");
      return;
    }

    // Clear packet
    packet.data.resize(0);
    packet.wrapped.resize(0);

    // Set origin
    packet.header.orig = NODES::GROUND_NODE_ID;

    // Get destination
    if (NodeType.find(args[0]) != NodeType.end())
    {
      packet.header.dest = NodeType.find(args[0])->second;
    }
    else
    {
      Serial.println("Invalid Node Destination. Options=[artemis_teensy, artemis_rpi, pleiades]");
      return;
    }

    // Parse radio_out and radio_in
    if (args[1].find(':') == string::npos)
    {
      Serial.println("Invalid radio argument format: <radio_out>:<radio_in>");
      return;
    }
    const char *radios = args[1].c_str();
    char *radio_arg = strtok((char *)radios, ":");
    String radio_out(radio_arg);
    radio_arg = strtok(NULL, "");
    String radio_in(radio_arg);
    radio_in = radio_in.toLowerCase();
    if (RadioType.find(radio_in.c_str()) != RadioType.end())
    {
      packet.header.radio = RadioType.find(radio_in.c_str())->second;
    }
    else
    {
      sprintf(buf, "Invalid radio_in: %s", radio_in.c_str());
      Serial.println(buf);
      memset(buf, 0, sizeof(buf));
      return;
    }

    // Parse command
    if (packet.StringType.find(args[2]) != packet.StringType.end())
    {
      packet.header.type = packet.StringType.find(args[2])->second;
    }
    else
    {
      Serial.println("Invalid Command Type");
      return;
    }

    switch (packet.header.type)
    {
    case PacketComm::TypeId::CommandEpsSwitchName:
    {
      if (args.size() < 5)
      {
        Serial.println("Incorrect command, usage: EpsSwitchName <switch name> 1|0");
        return;
      }
      String switch_name(args[3].c_str());
      switch_name = switch_name.toLowerCase();
      if (PDU_SW_Type.find(switch_name.c_str()) != PDU_SW_Type.end())
      {
        packet.data.push_back((uint8_t)PDU_SW_Type.find(switch_name.c_str())->second);
      }
      else
      {
        Serial.println("Invalid PDU Switch");
        return;
      }
      if (args[4] == "1")
      {
        packet.data.push_back((uint8_t)1);
      }
      else if (args[4] == "0")
      {
        packet.data.push_back((uint8_t)0);
      }
      else
      {
        Serial.println("Incorrect command, usage: EpsSwitchName <switch name> 1|0");
        return;
      }
      break;
    }
    case PacketComm::TypeId::CommandEpsSwitchStatus:
    {
      if (args.size() < 4)
      {
        Serial.println("Incorrect command, usage: EpsSwitchStatus <switch name>");
        return;
      }
      String switch_name(args[3].c_str());
      switch_name = switch_name.toLowerCase();
      if (PDU_SW_Type.find(switch_name.c_str()) != PDU_SW_Type.end())
      {
        packet.data.push_back((uint8_t)PDU_SW_Type.find(switch_name.c_str())->second);
      }
      else
      {
        Serial.println("Invalid PDU Switch");
        return;
      }
      break;
    }
    default:
      break;
    }

    // Store data
    // if (args.size() > 3)
    // {
    //   for (size_t i = 3; i < args.size(); i++)
    //   {
    //     for (size_t j = 0; j < args[i].size(); j++)
    //     {
    //       packet.data.push_back(args[i][j]);
    //     }
    //     if (i != args.size() - 1)
    //       packet.data.push_back(' ');
    //   }
    // }

    // Send to ground station radio
    radio_out = radio_out.toLowerCase();
    if (RadioType.find(radio_out.c_str()) != RadioType.end())
    {
      ARTEMIS_RADIOS radio_send = RadioType.find(radio_out.c_str())->second;
      switch (radio_send)
      {
      case ARTEMIS_RADIOS::RFM23:
      {
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        break;
      }
      case ARTEMIS_RADIOS::ASTRODEV:
      {
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
        break;
      }
      default:
      {
        sprintf(buf, "Radio [%s] is not a valid ground station radio", radio_out.c_str());
        Serial.println(buf);
        memset(buf, 0, sizeof(buf));
        return;
      }
      }
    }
    else
    {
      sprintf(buf, "Invalid radio_out: %s", radio_out.c_str());
      Serial.println(buf);
      memset(buf, 0, sizeof(buf));
      return;
    }

    Serial.println("Command sent to radio. Waiting to send...");
  }
}
