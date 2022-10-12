#include <SPI.h>
#include <RH_RF95.h>
#include <RHHardwareSPI1.h>

/* RFM98 FREQUENCY CONFIG */
#define RFM98_FREQ   434.0

/* RFM98 PIN CONFIG */
#define RFM98_CS_PIN  37
#define RFM98_INT_PIN 30
#define RFM98_RST_PIN 38

/* SPI MISO/MOSI/SCK CONFIG */
#define RFM98_SPI_MISO 39
#define RFM98_SPI_MOSI 26
#define RFM98_SPI_SCK 27

/* RFM98 Power Level */
#define RFM98_TX_POWER 23

namespace Artemis {
    namespace Teensy {
        namespace Radio {
            class RFM98 {
                private:
                    RH_RF95 rfm98;
                    uint8_t RFM98_SEND_BUF[62];
                    uint8_t RFM98_SEND_LEN = 0;
                    uint8_t RFM98_RECV_BUF[RH_RF95_MAX_MESSAGE_LEN];
                    uint8_t RFM98_RECV_LEN = sizeof(RFM98_RECV_BUF);
                public:
                    RFM98(uint8_t slaveSelectPin = RFM98_CS_PIN, uint8_t interruptPin = RFM98_INT_PIN, RHGenericSPI& spi = hardware_spi1);
                    void RFM98_RESET();
                    void RFM98_INIT();
                    void RFM98_SEND(const char *input);
                    void RFM98_RECV();
            };
        }
    }
}



