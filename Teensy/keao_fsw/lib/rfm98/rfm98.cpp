#include "rfm98.h"

namespace Artemis
{
    namespace Devices
    {
        namespace Radios
        {
            RFM98::RFM98(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm98(slaveSelectPin, interruptPin, spi) {}

            RFM98::~RFM98()
            {
                gcm.clear();
            }

            int32_t RFM98::init(rfm98_config cfg, Threads::Mutex *mtx)
            {
                Serial.println("[RFM98] Initializing ...");
                config = cfg;
                spi_mtx = mtx;

                // Set up AES-256 encryption
                gcm.setKey(AES_256_KEY, strlen(AES_256_KEY));

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
                int32_t iretn = 0;

                iretn = packet.Wrap();
                if (iretn < 0)
                {
                    Serial.println("[RFM98] Wrap fail");
                    return -1;
                }

                // Encrypt
                vector<uint8_t> encrypted;
                vector<uint8_t> iv;
                RNG.rand(iv.data(), AES_IV_SIZE);
                gcm.encrypt(encrypted.data(), packet.wrapped.data(), packet.wrapped.size());
                encrypted.insert(encrypted.end(), iv.begin(), iv.end());

                if (encrypted.size() > RH_RF95_MAX_MESSAGE_LEN)
                {
                    Serial.println("[RFM98] Packet Size Overflow");
                    return -1;
                }

                Threads::Scope lock(*spi_mtx);
                rfm98.send(encrypted.data(), encrypted.size());
                rfm98.waitPacketSent(100);

                rfm98.setModeIdle();
                Serial.print("[RFM98] SENDING: [");
                for (size_t i = 0; i < encrypted.size(); i++)
                {
                    Serial.print(encrypted[i]);
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
                    vector<uint8_t> encrypted;
                    encrypted.resize(RH_RF95_MAX_MESSAGE_LEN);
                    uint8_t bytes_received = encrypted.size();
                    if (rfm98.recv(encrypted.data(), &bytes_received))
                    {
                        encrypted.resize(bytes_received);

                        // Decrypt
                        vector<uint8_t> ciphertext;
                        vector<uint8_t> iv;
                        for (size_t i = 0; i < encrypted.size() - AES_IV_SIZE; i++)
                        {
                            ciphertext.push_back(encrypted[i]);
                        }
                        for (size_t i = encrypted.size() - AES_IV_SIZE; i < encrypted.size(); i++)
                        {
                            iv.push_back(encrypted[i]);
                        }
                        packet.wrapped.clear();
                        gcm.setIV(iv.data(), iv.size());
                        gcm.decrypt(packet.wrapped.data(), ciphertext.data(), ciphertext.size());

                        // packet.wrapped.resize(bytes_received);
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
