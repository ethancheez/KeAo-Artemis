#include <rfm23.h>

namespace Artemis
{
    namespace Devices
    {
        namespace Radios
        {
            RFM23::RFM23(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm23(slaveSelectPin, interruptPin, spi) {}

            int32_t RFM23::init(rfm23_config cfg, Threads::Mutex *mtx)
            {
                config = cfg;
                spi_mtx = mtx;

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
                rfm23.setFrequency(config.freq);   // frequency default is 434MHz
                rfm23.setTxPower(config.tx_power); // 20 is the max

                /* RFM23 modulation     schemes and data rates
                 * <FSK_Rb125Fd125>     highest FSK data rate (125kbs)
                 * <FSK_Rb2Fd5>         lowe FSK data rate (2kbs)
                 * <GFSK_Rb125Fd125>    highest GFSK rate GFSK (125kbs)
                 * <GFSK_Rb2Fd5>        lowest GFSK data rate (2kbs)
                 * <FSK_Rb_512Fd2_5>    original FSK test modulation (0.512kbs)
                 */

                rfm23.sleep();
                timeout = 0;
                while (!rfm23.setModemConfig(RH_RF22::GFSK_Rb125Fd125))
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
                digitalWrite(config.pins.rx_on, LOW);
                digitalWrite(config.pins.tx_on, LOW);

                packet.wrapped.resize(0);
                packet.Wrap();

                Threads::Scope lock(*spi_mtx);
                rfm23.send(packet.wrapped.data(), packet.wrapped.size());
                rfm23.waitPacketSent();

                rfm23.sleep();
                rfm23.setModeIdle();
                Serial.print("[RFM23] SENDING: [");
                for (size_t i = 0; i < packet.wrapped.size(); i++)
                {
                    Serial.print(packet.wrapped[i], HEX);
                }
                Serial.println("]");

                return 0;
            }

            int32_t RFM23::recv(PacketComm &packet)
            {
                int32_t iretn = 0;

                digitalWrite(config.pins.rx_on, LOW);
                digitalWrite(config.pins.tx_on, HIGH);

                Threads::Scope lock(*spi_mtx);
                if (rfm23.waitAvailableTimeout(100))
                {
                    packet.wrapped.resize(0);
                    packet.wrapped.resize(RH_RF22_MAX_MESSAGE_LEN);
                    uint8_t bytes_recieved = packet.wrapped.size();
                    if (rfm23.recv(packet.wrapped.data(), &bytes_recieved))
                    {
                        packet.wrapped.resize(bytes_recieved);
                        iretn = packet.Unwrap();
                        rfm23.setModeIdle();

                        if (iretn < 0)
                            return -1;

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
        }
    }
}