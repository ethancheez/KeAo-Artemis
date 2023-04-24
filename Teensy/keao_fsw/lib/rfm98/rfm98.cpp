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
                Serial.println("[RFM98] Initializing ...");
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

                crypto.setKey(config.key);

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

            int32_t RFM98::send(PacketComm &packet, bool encrypt)
            {
                int32_t iretn = 0;

                packet.wrapped.clear();
                iretn = packet.Wrap();
                if (iretn < 0)
                {
                    Serial.println("[RFM98] Wrap fail");
                    return -1;
                }

                vector<uint8_t> payload;
                if (encrypt)
                {
                    crypto.encrypt(packet.wrapped, payload, config.iv_size);
                }
                else
                {
                    payload = packet.wrapped;
                }

                if (payload.size() > RH_RF95_MAX_MESSAGE_LEN)
                {
                    return COSMOS_GENERAL_ERROR_OVERSIZE;
                }

                Threads::Scope lock(*spi_mtx);
                rfm98.send(payload.data(), payload.size());
                iretn = rfm98.waitPacketSent(1000);

                if (iretn == false)
                {
                    Serial.println("[RFM98] TX Timeout");
                    return -1;
                }

                rfm98.setModeIdle();
                Serial.print("[RFM98] SENDING: [");
                for (size_t i = 0; i < payload.size(); i++)
                {
                    Serial.print(payload[i]);
                }
                Serial.println("]");

                return 0;
            }

            int32_t RFM98::recv(PacketComm &packet, uint16_t timeout, bool decrypt)
            {
                int32_t iretn = 0;

                Threads::Scope lock(*spi_mtx);
                if (rfm98.waitAvailableTimeout(timeout))
                {
                    vector<uint8_t> payload;
                    payload.resize(RH_RF95_MAX_MESSAGE_LEN);
                    uint8_t bytes_received = payload.size();
                    if (rfm98.recv(payload.data(), &bytes_received))
                    {
                        payload.resize(bytes_received);

                        if (decrypt)
                        {
                            crypto.decrypt(payload, packet.wrapped, config.iv_size);
                        }
                        else
                        {
                            packet.wrapped = payload;
                        }

                        iretn = packet.Unwrap();
                        rfm98.setModeIdle();

                        if (iretn < 0)
                        {
                            Serial.println("[RFM98] Unwrap fail");
                            return -1;
                        }

                        return packet.wrapped.size();
                    }
                }
                rfm98.setModeIdle();
                return -1;
            }
        }
    }
}
