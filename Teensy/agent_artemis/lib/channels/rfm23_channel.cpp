#include <artemis_channels.h>

namespace
{
    Artemis::Teensy::Radio::RFM23 rfm23(RFM23_CS_PIN, RFM23_INT_PIN, hardware_spi1);
    PacketComm packet;
}

void Artemis::Teensy::Channels::rfm23_channel()
{
    int retries = 20;
    while (!rfm23.init())
    {
        retries--;
        if (retries == 0)
        {
            kill_thread((char *)"rfm23 thread");
        }
    }

    while (true)
    {
        if (PullQueue(packet, rfm23_queue, rfm23_queue_mtx))
        {
            switch (packet.header.type)
            {
            case PacketComm::TypeId::DataBeacon:
            case PacketComm::TypeId::DataPong:
            case PacketComm::TypeId::DataEpsResponse:
            case PacketComm::TypeId::DataRadioResponse:
            case PacketComm::TypeId::DataAdcsResponse:
            case PacketComm::TypeId::DataResponse:
                rfm23.send(packet);
                break;
            default:
                break;
            }
        }

        packet.data.resize(0);
        if (rfm23.recv(&packet))
        {
            Serial.print("[RFM23] RECEIVED: [");
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
