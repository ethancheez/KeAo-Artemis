#include <artemis_channels.h>

Artemis::Teensy::Radio::RFM98 rfm98(RFM98_CS_PIN, RFM98_INT_PIN, hardware_spi1);
PacketComm packet;

void Artemis::Teensy::Channels::rfm98_channel()
{
    while (!rfm98.init())
        ;

    while (true)
    {
        if (PullQueue(&packet, rfm98_queue, rfm98_queue_mtx))
        {
            packet.Wrap();
            for (size_t i = 0; i < packet.wrapped.size(); i++)
            {
                Serial.print(packet.wrapped[i], HEX);
            }
            Serial.println();
            rfm98.send(packet.wrapped.data(), packet.wrapped.size());
        }

        rfm98.recv();
    }
}
