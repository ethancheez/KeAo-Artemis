#include <artemis_channels.h>

void Artemis::Teensy::Channels::rfm98_channel()
{
    Artemis::Teensy::Radio::RFM98 rfm98(RFM98_CS_PIN, RFM98_INT_PIN, hardware_spi1);
    PacketComm packet;

    // rfm98.RFM98_INIT();

    while (true)
    {
        if (packet.PullQueue(rfm98_queue, rfm98_queue_mtx))
        {
        }
    }
}