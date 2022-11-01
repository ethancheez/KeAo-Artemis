#include <artemis_channels.h>

Artemis::Teensy::Radio::RFM23 rfm23(RFM23_CS_PIN, RFM23_INT_PIN, hardware_spi1);
PacketComm packet;

void Artemis::Teensy::Channels::rfm23_channel()
{
    while (!rfm23.init())
        ;

    while (true)
    {
        if (PullQueue(&packet, rfm23_queue, rfm23_queue_mtx))
        {
            Serial.println((int)packet.header.type);
        }

        packet = rfm23.recv();

        // Got data from radio
        if (packet.packetized.size() > 0)
        {
            Serial.println("rfm23 recv");
            packet.packetized.resize(0);
        }
    }
}
