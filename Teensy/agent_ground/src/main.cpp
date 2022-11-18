#include <Arduino.h>
#include <vector>
#include <map>
#include <string.h>
#include <artemis_channels.h>
#include <support/configCosmosKernel.h>

void handle_cmd();

namespace
{
  char buf[256];
  PacketComm packet;
}

void setup()
{
  Serial.begin(115200);
  delay(3000);

  threads.setSliceMillis(10);

  // Threads
  thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm23_channel), "rfm23 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm98_channel), "rfm98 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::astrodev_channel), "astrodev thread"});
}

void loop()
{
  handle_cmd();
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
      Serial.println("COMMAND SYNTAX: <node_dest> <radio_out>:<radio_in> <command> [data]");
      return;
    }

    // Clear packet
    packet.data.resize(0);
    packet.wrapped.resize(0);

    // Get destination
    if (NodeType.find(args[0]) != NodeType.end())
    {
      packet.header.dest = NodeType.find(args[0])->second;
    }
    else
    {
      Serial.println("Invalid Node Destination. Options=[artemis_teensy, artemis_rpi]");
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

    // Store data
    if (args.size() > 3)
    {
      for (size_t i = 3; i < args.size(); i++)
      {
        for (size_t j = 0; j < args[i].size(); j++)
        {
          packet.data.push_back(args[i][j]);
        }
        if (i != args.size() - 1)
          packet.data.push_back(' ');
      }
    }

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
