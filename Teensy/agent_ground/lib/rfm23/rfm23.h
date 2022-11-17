#ifndef _RFM23_H
#define _RFM23_H

// #include <SPI.h>
#include <RH_RF22.h>
#include <RHHardwareSPI1.h>
#include <artemis_defs.h>

#undef RH_RF22_MAX_MESSAGE_LEN
#define RH_RF22_MAX_MESSAGE_LEN 64

/* RFM23 FREQUENCY CONFIG */
#define RFM23_FREQ 433.0

/* RFM23 PIN CONFIG */
#define RFM23_CS_PIN SPI1_CS1
#define RFM23_INT_PIN NIRQ

/* SPI MISO/MOSI/SCK CONFIG */
#define RFM23_SPI_MISO SPI1_D0
#define RFM23_SPI_MOSI SPI1_D1
#define RFM23_SPI_SCK SPI1_SCLK

/* RFM23 Power Level */
#define RFM23_TX_POWER 20
#define RFM23_RX_ON RX_ON
#define RFM23_TX_ON TX_ON

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            class RFM23
            {
            private:
                RH_RF22 rfm23;

            public:
                RFM23(uint8_t slaveSelectPin = RFM23_CS_PIN, uint8_t interruptPin = RFM23_INT_PIN, RHGenericSPI &spi = hardware_spi1);
                void reset();
                bool init();
                void send(PacketComm &packet);
                bool recv(PacketComm &packet);
            };
        }
    }
}

#endif // _RFM23_H
