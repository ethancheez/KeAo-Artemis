#include "channels/artemis_channels.h"
#include <rfm98.h>
#include "private.h"

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
        .key = (uint8_t *)AES_256_KEY,
        .iv_size = AES_IV_SIZE,
    };

    RFM98 rfm98(config.pins.cs, config.pins.nirq, hardware_spi1);
    PacketComm packet;
}

void Artemis::Teensy::Channels::rfm98_channel()
{
    while (rfm98.init(config, &spi1_mtx) < 0)
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

        int32_t timeout = 5000 - rfm23_queue.size() * 1000;
        if (timeout < 100)
            timeout = 100;
        if (rfm98.recv(packet, (uint16_t)timeout) >= 0)
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
