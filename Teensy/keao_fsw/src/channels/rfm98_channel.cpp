#include "channels/artemis_channels.h"
#include <rfm98.h>

namespace
{
    using namespace Artemis::Devices::Radios;

    RFM98::rfm98_config config = {
        .freq = 433,
        .tx_power = 23,
        .pins = {
            .spi_miso = SPI1_D0,
            .spi_mosi = SPI1_D1,
            .spi_sck = SPI1_SCLK,
            .nirq = T_GPIO2,
            .cs = T_CS1,
            .reset = T_GPIO3,
        },
    };

    RFM98 rfm98(config.pins.cs, config.pins.nirq, hardware_spi1);
    PacketComm packet;
}

void Artemis::Teensy::Channels::rfm98_channel()
{
    while (!rfm98.init(config, &spi1_mtx))
        ;

    while (true)
    {
        if (PullQueue(packet, rfm98_queue, rfm98_queue_mtx))
        {
            switch (packet.header.type)
            {
            case PacketComm::TypeId::DataObcBeacon:
            case PacketComm::TypeId::DataObcPong:
            case PacketComm::TypeId::DataEpsResponse:
            case PacketComm::TypeId::DataRadioResponse:
            case PacketComm::TypeId::DataAdcsResponse:
            case PacketComm::TypeId::DataObcResponse:
            {
                rfm98.send(packet);
                threads.delay(500);
                break;
            }
            default:
                break;
            }
        }

        if (rfm98.recv(packet) >= 0)
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
