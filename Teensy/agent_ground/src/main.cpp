#include <Arduino.h>
#include <vector>
#include <artemis_channels.h>
#include <support/configCosmos.h>
#include <USBHost_t36.h>
#include "artemisbeacons.h"

namespace
{
  PacketComm packet;
  USBHost usb;
}

void setup()
{
  Serial.begin(115200);
  usb.begin();
  delay(3000);

  threads.setSliceMillis(10);

  // Threads
  thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm23_channel), "rfm23 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm98_channel), "rfm98 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::pdu_channel), "pdu thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::astrodev_channel), "astrodev thread"});
}

void loop()
{
  packet.header.orig = NODES::GROUND_NODE_ID;
  packet.header.dest = NODES::TEENSY_NODE_ID;
  packet.header.radio = ARTEMIS_RADIOS::RFM23;
  packet.header.type = PacketComm::TypeId::CommandPing;
  packet.data.resize(0);
  const char *data = "Ping";
  for (size_t i = 0; i < strlen(data); i++)
  {
    packet.data.push_back(data[i]);
  }
  PushQueue(packet, main_queue, main_queue_mtx);
  threads.delay(1000);

  if (PullQueue(packet, main_queue, main_queue_mtx))
  {
    if (packet.header.dest == NODES::TEENSY_NODE_ID)
    {
      switch (packet.header.radio)
      {
      case ARTEMIS_RADIOS::RFM23:
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        break;
      case ARTEMIS_RADIOS::ASTRODEV:
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
        break;
      default:
        break;
      }
    }
  }
  threads.delay(10);
}
