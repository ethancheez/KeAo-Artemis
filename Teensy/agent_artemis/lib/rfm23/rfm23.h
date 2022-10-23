#ifndef _RFM23_H
#define _RFM23_H

#include <SPI.h>
#include <RH_RF22.h>
#include <RHHardwareSPI1.h>
#include <teensy_pins.h>
#include <cmd_queue.h>

/* RFM23 FREQUENCY CONFIG */
#define RFM23_FREQ   430.0

/* RFM23 PIN CONFIG */
#define RFM23_CS_PIN  SPI1_CS1
#define RFM23_INT_PIN NIRQ

/* SPI MISO/MOSI/SCK CONFIG */
#define RFM23_SPI_MISO SPI1_D0
#define RFM23_SPI_MOSI SPI1_D1
#define RFM23_SPI_SCK SPI1_SCLK

/* RFM23 Power Level */
#define RFM23_TX_POWER 20
#define RFM23_RX_ON RX_ON
#define RFM23_TX_ON TX_ON

namespace Artemis {
    namespace Teensy {
        namespace Radio {
            class RFM23 {
                private:
                    RH_RF22 rfm23;
                    uint8_t RFM23_RECV_BUF[RH_RF22_MAX_MESSAGE_LEN];
                    uint8_t RFM23_RECV_LEN = sizeof(RFM23_RECV_BUF);
                public:
                    RFM23(uint8_t slaveSelectPin = RFM23_CS_PIN, uint8_t interruptPin = RFM23_INT_PIN, RHGenericSPI& spi = hardware_spi1);
                    void RFM23_RESET();
                    bool RFM23_INIT();
                    void RFM23_SEND(const char *input);
                    void RFM23_RECV();
            };
        }
    }
}

#endif // _RFM23_H
