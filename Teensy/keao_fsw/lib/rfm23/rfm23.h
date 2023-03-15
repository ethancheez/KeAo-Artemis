#ifndef _RFM23_H
#define _RFM23_H

#include <RH_RF22.h>
#include <RHHardwareSPI1.h>
#include <TeensyThreads.h>
#include <support/packetcomm.h>
#include <cosmos-crypto.h>

#undef RH_RF22_MAX_MESSAGE_LEN
#define RH_RF22_MAX_MESSAGE_LEN 50

namespace Artemis
{
    namespace Devices
    {
        namespace Radios
        {
            class RFM23
            {
            public:
                struct __attribute__((packed)) rfm23_config
                {
                    uint16_t freq;
                    uint8_t tx_power;

                    struct
                    {
                        uint8_t spi_miso;
                        uint8_t spi_mosi;
                        uint8_t spi_sck;
                        uint8_t nirq;
                        uint8_t cs;
                        uint8_t tx_on;
                        uint8_t rx_on;
                    } pins;

                    const char *key;
                };

                RFM23(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi = hardware_spi1);
                int32_t reset();
                int32_t init(rfm23_config cfg, Threads::Mutex *mtx);
                int32_t send(PacketComm &packet);
                int32_t recv(PacketComm &packet, uint16_t timeout);
                int32_t set_tx_power(uint8_t power);
                int32_t get_tsen();

            private:
                RH_RF22 rfm23;
                Artemis::Teensy::Crypto crypto;

                Threads::Mutex *spi_mtx;
                rfm23_config config;
            };
        }
    }
}

#endif // _RFM23_H
