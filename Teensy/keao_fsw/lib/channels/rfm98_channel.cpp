#include <artemis_channels.h>

namespace
{
    Artemis::Teensy::Radio::RFM98 rfm98(RFM98_CS_PIN, RFM98_INT_PIN, hardware_spi1);
    PacketComm packet;
}

void Artemis::Teensy::Channels::rfm98_channel()
{
    while (!rfm98.init())
        ;

    while (true)
    {
        if (PullQueue(packet, rfm98_queue, rfm98_queue_mtx))
        {
            switch (packet.header.type)
            {
            case PacketComm::TypeId::DataBeacon:
            case PacketComm::TypeId::DataPong:
            case PacketComm::TypeId::DataEpsResponse:
            case PacketComm::TypeId::DataRadioResponse:
            case PacketComm::TypeId::DataAdcsResponse:
            case PacketComm::TypeId::DataResponse:
            {
                rfm98.send(packet);
                threads.delay(500);
                break;
            }
            default:
                break;
            }
        }

        packet.data.resize(0);
        if (rfm98.recv(packet))
        {
            Serial.print("[RFM98] RECEIVED: [");
            for (size_t i = 0; i < packet.data.size(); i++)
            {
                Serial.print((char)packet.data[i]);
            }
            Serial.println("]");
            PushQueue(packet, main_queue, main_queue_mtx);
        }
        threads.delay(10);
    }
}
