#include <rfm23.h>

namespace Artemis
{
    namespace Devices
    {
        namespace Radios
        {
            RFM23::RFM23(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm23(slaveSelectPin, interruptPin, spi) {}

            RFM23::~RFM23()
            {
                gcm.clear();
            }

            int32_t RFM23::init(rfm23_config cfg, Threads::Mutex *mtx)
            {
                Serial.println("[RFM23] Initializing ...");
                config = cfg;
                spi_mtx = mtx;

                // Set up AES-256 encryption
                gcm.setKey(AES_256_KEY, strlen(AES_256_KEY));

                Threads::Scope lock(*spi_mtx);
                SPI1.setMISO(config.pins.spi_miso);
                SPI1.setMOSI(config.pins.spi_mosi);
                SPI1.setSCK(config.pins.spi_sck);
                pinMode(config.pins.rx_on, OUTPUT);
                pinMode(config.pins.tx_on, OUTPUT);

                elapsedMillis timeout;
                while (!rfm23.init())
                {
                    if (timeout > 10000)
                    {

                        Serial.println("[RFM23] INIT FAILED");
                        return -1;
                    }
                }
                rfm23.setFrequency(config.freq);
                rfm23.setTxPower(config.tx_power);

                /* RFM23 modulation     schemes and data rates
                 * <FSK_Rb125Fd125>     highest FSK data rate (125kbs)
                 * <FSK_Rb2Fd5>         lowe FSK data rate (2kbs)
                 * <GFSK_Rb125Fd125>    highest GFSK rate GFSK (125kbs)
                 * <GFSK_Rb2Fd5>        lowest GFSK data rate (2kbs)
                 * <FSK_Rb_512Fd2_5>    original FSK test modulation (0.512kbs)
                 */

                rfm23.sleep();
                timeout = 0;
                while (!rfm23.setModemConfig(RH_RF22::FSK_Rb2Fd5))
                {
                    if (timeout > 10000)
                    {
                        Serial.println("[RFM23] SET FSK MODULATION FAILED");
                        return -1;
                    }
                }
                rfm23.sleep();

                Serial.println("[RFM23] INIT SUCCESS");
                rfm23.setModeIdle();
                return 0;
            }

            int32_t RFM23::reset()
            {
                Threads::Scope lock(*spi_mtx);
                rfm23.reset();
                return 0;
            }

            int32_t RFM23::send(PacketComm &packet)
            {
                int32_t iretn = 0;

                digitalWrite(config.pins.rx_on, HIGH);
                digitalWrite(config.pins.tx_on, LOW);

                packet.wrapped.resize(0);
                iretn = packet.Wrap();
                if (iretn < 0)
                {
                    Serial.println("[RFM23] Wrap fail");
                    return -1;
                }

                // Encrypt
                vector<uint8_t> encrypted;
                vector<uint8_t> iv;
                RNG.rand(iv.data(), AES_IV_SIZE);
                gcm.encrypt(encrypted.data(), packet.wrapped.data(), packet.wrapped.size());
                encrypted.insert(encrypted.end(), iv.begin(), iv.end());

                if (encrypted.size() > RH_RF22_MAX_MESSAGE_LEN)
                {
                    Serial.println("[RFM23] Packet Size Overflow");
                    return -1;
                }

                Threads::Scope lock(*spi_mtx);
                rfm23.send(encrypted.data(), encrypted.size());
                rfm23.waitPacketSent(100);

                rfm23.sleep();
                rfm23.setModeIdle();
                Serial.print("[RFM23] SENDING: [");
                for (size_t i = 0; i < encrypted.size(); i++)
                {
                    Serial.print(encrypted[i], HEX);
                }
                Serial.println("]");

                return 0;
            }

            int32_t RFM23::recv(PacketComm &packet, uint16_t timeout)
            {
                int32_t iretn = 0;

                digitalWrite(config.pins.rx_on, LOW);
                digitalWrite(config.pins.tx_on, HIGH);

                Threads::Scope lock(*spi_mtx);
                if (rfm23.waitAvailableTimeout(timeout))
                {
                    vector<uint8_t> encrypted;
                    encrypted.resize(RH_RF22_MAX_MESSAGE_LEN);
                    uint8_t bytes_received = encrypted.size();
                    if (rfm23.recv(encrypted.data(), &bytes_received))
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

                        // packet.wrapped.resize(bytes_recieved);
                        iretn = packet.Unwrap();
                        rfm23.setModeIdle();

                        if (iretn < 0)
                        {
                            Serial.println("[RFM23] Unwrap fail");
                            return -1;
                        }

                        return packet.wrapped.size();
                    }
                }
                rfm23.setModeIdle();
                return -1;
            }

            int32_t RFM23::set_tx_power(uint8_t power)
            {
                if (power > 20)
                    power = 20;
                else if (power < 1)
                    power = 1;

                config.tx_power = power;
                return 0;
            }

            int32_t RFM23::get_tsen()
            {
                return rfm23.temperatureRead();
            }
        }
    }
}