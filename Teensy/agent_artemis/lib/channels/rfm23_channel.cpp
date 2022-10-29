#include "artemis_channels.h"

void Artemis::Teensy::Channels::rfm23_channel()
{
    Artemis::Teensy::Radio::RFM23 rfm23(RFM23_CS_PIN, RFM23_INT_PIN, hardware_spi1);
    PacketComm packet;

    // rfm23.RFM23_INIT();

    while (true)
    {
        if (packet.PullQueue(rfm23_queue, rfm23_queue_mtx))
        {
            Serial.println((int)packet.header.orig);
            Serial.println((int)packet.header.dest);
            Serial.println((int)packet.header.radio);
            Serial.println((int)packet.header.type);
        }

        rfm23.RFM23_RECV();
        // rfm23.RFM23_SEND("hi", 2);
        // delay(5000);
    }
}