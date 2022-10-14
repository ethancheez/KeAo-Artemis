#include <Arduino.h>
#include <vector>
#include <artemis_channels.h>

struct thread_struct
{
  int thread_id;
  const char *thread_name;
};

// Max thread size = 16
std::vector<struct thread_struct> thread_list;

void setup()
{
  Serial.begin(115200);

  // Threads
  thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::pdu_channel), "pdu thread"});
  thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm98_channel), "rfm98 thread"});
}

void loop()
{
}
