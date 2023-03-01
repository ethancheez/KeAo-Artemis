#ifndef _RFM98_H
#define _RFM98_H

#include <RH_RF95.h>
#include <RHHardwareSPI1.h>
#include <TeensyThreads.h>
#include <GCM.h>
#include <AES.h>
#include <RNG.h>
#include <support/packetcomm.h>

namespace Artemis
{
    namespace Devices
    {
        namespace Radios
        {
            class RFM98
            {
            public:
                struct __attribute__((packed)) rfm98_config
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
                        uint8_t reset;
                    } pins;

                    const uint8_t *key;
                    uint8_t iv_size;
                };

                RFM98(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi = hardware_spi1);
                ~RFM98();
                int32_t reset();
                int32_t init(rfm98_config cfg, Threads::Mutex *mtx);
                int32_t send(PacketComm &packet);
                int32_t recv(PacketComm &packet, uint16_t timeout);

            private:
                RH_RF95 rfm98;
                GCM<AES256> gcm;

                Threads::Mutex *spi_mtx;
                rfm98_config config;
            };
        }
    }
}

#endif // _RFM98_H
