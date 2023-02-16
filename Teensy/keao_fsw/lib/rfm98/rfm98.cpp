#include "rfm98.h"

namespace Artemis
{
    namespace Devices
    {
        namespace Radios
        {
            RFM98::RFM98(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm98(slaveSelectPin, interruptPin, spi) {}

            int32_t RFM98::init(rfm98_config cfg, Threads::Mutex *mtx)
            {
                config = cfg;
                spi_mtx = mtx;

                Threads::Scope lock(*spi_mtx);
                SPI1.setMISO(config.pins.spi_miso);
                SPI1.setMOSI(config.pins.spi_mosi);
                SPI1.setSCK(config.pins.spi_sck);

                pinMode(config.pins.reset, OUTPUT);
                digitalWrite(config.pins.reset, LOW);

                reset();

                elapsedMillis timeout;
                while (!rfm98.init())
                {
                    if (timeout > 10000)
                    {
                        Serial.println("[RFM98] INIT FAILED");
                        return -1;
                    }
                }

                rfm98.setFrequency(config.freq);
                rfm98.setTxPower(config.tx_power, false);

                Serial.println("[RFM98] INIT SUCCESS");
                rfm98.sleep();
                rfm98.setModeIdle();
                return 0;
            }

            int32_t RFM98::reset()
            {
                digitalWrite(config.pins.reset, LOW);
                threads.delay(10);
                digitalWrite(config.pins.reset, HIGH);
                threads.delay(10);
                return 0;
            }

            int32_t RFM98::send(PacketComm &packet)
            {
                packet.Wrap();

                Threads::Scope lock(*spi_mtx);
                rfm98.send(packet.wrapped.data(), packet.wrapped.size());
                rfm98.waitPacketSent();
                rfm98.sleep();
                rfm98.setModeIdle();
                Serial.print("[RFM98] SENDING: [");
                for (size_t i = 0; i < packet.wrapped.size(); i++)
                {
                    Serial.print(packet.wrapped[i]);
                }
                Serial.println("]");

                return 0;
            }

            int32_t RFM98::recv(PacketComm &packet, uint16_t timeout)
            {
                int32_t iretn = 0;

                Threads::Scope lock(*spi_mtx);
                if (rfm98.waitAvailableTimeout(timeout))
                {
                    packet.wrapped.resize(RH_RF95_MAX_MESSAGE_LEN);
                    uint8_t bytes_received = packet.wrapped.size();
                    if (rfm98.recv(packet.wrapped.data(), &bytes_received))
                    {
                        packet.wrapped.resize(bytes_received);
                        iretn = packet.Unwrap();
                        rfm98.setModeIdle();

                        if (iretn < 0)
                            return -1;

                        return packet.wrapped.size();
                    }
                }
                rfm98.setModeIdle();
                return -1;
            }
        }
    }
}
