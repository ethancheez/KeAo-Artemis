#include "artemis_channels.h"

void Artemis::Teensy::Channels::astrodev_channel()
{
    Cosmos::Support::PacketComm packet;

    packet.packet = (char*)"test";
    packet.PushQueue(main_queue, main_queue_mtx);
}
