#include "channels/artemis_channels.h"
#include <rfm23.h>
#include "private.h"

namespace
{
    using namespace Artemis::Devices::Radios;

    RFM23::rfm23_config config = {
        .freq = 433,
        .tx_power = RH_RF22_RF23BP_TXPOW_30DBM,
        .pins = {
            .spi_miso = SPI1_D0,
            .spi_mosi = SPI1_D1,
            .spi_sck = SPI1_SCLK,
            .nirq = NIRQ,
            .cs = SPI1_CS1,
            .tx_on = TX_ON,
            .rx_on = RX_ON,
        },
        .key = (uint8_t *)AES_256_KEY,
        .iv_size = AES_IV_SIZE,
    };

    RFM23 rfm23(config.pins.cs, config.pins.nirq, hardware_spi1);
    PacketComm packet;
    elapsedMillis telem;
}

void Artemis::Teensy::Channels::rfm23_channel()
{
    while (rfm23.init(config, &spi1_mtx) < 0)
        ;

    while (true)
    {
        if (PullQueue(packet, rfm23_queue, rfm23_queue_mtx))
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
                rfm23.send(packet);
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
        if (rfm23.recv(packet, (uint16_t)timeout) >= 0)
        {
            Serial.print("[RFM23] RECEIVED: [");
            for (size_t i = 0; i < packet.data.size(); i++)
            {
                Serial.print(packet.data[i], HEX);
            }
            Serial.println("]");
            PushQueue(packet, main_queue, main_queue_mtx);
        }

        if (telem > 10000)
        {
            Serial.print("[RFM23] TSEN = ");
            Serial.println(rfm23.get_tsen());
            telem = 0;
        }

        threads.delay(10);
    }
}
