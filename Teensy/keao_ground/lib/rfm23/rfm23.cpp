#include <rfm23.h>

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            RFM23::RFM23(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm23(slaveSelectPin, interruptPin, spi) {}

            bool RFM23::init()
            {
                Threads::Scope lock(spi1_mtx);
                SPI1.setMISO(RFM23_SPI_MISO);
                SPI1.setMOSI(RFM23_SPI_MOSI);
                SPI1.setSCK(RFM23_SPI_SCK);
                pinMode(RFM23_RX_ON, OUTPUT);
                pinMode(RFM23_TX_ON, OUTPUT);

                unsigned long timeoutStart = millis();
                while (!rfm23.init())
                {
                    if (millis() - timeoutStart > 10000)
                    {

                        Serial.println("[RFM23] INIT FAILED");
                        return false;
                    }
                }
                rfm23.setFrequency(RFM23_FREQ);   // frequency default is 434MHz
                rfm23.setTxPower(RFM23_TX_POWER); // 20 is the max

                /* RFM23 modulation schemes and data rates
                 * <FSK_Rb125Fd125>     highest FSK data rate (125kbs)
                 * <FSK_Rb2Fd5>         lowe FSK data rate (2kbs)
                 * <GFSK_Rb125Fd125>    highest GFSK rate GFSK (125kbs)
                 * <GFSK_Rb2Fd5>        lowest GFSK data rate (2kbs)
                 * <FSK_Rb_512Fd2_5>    original FSK test modulation (0.512kbs)
                 */

                rfm23.sleep();
                timeoutStart = millis();
                while (!rfm23.setModemConfig(RH_RF22::GFSK_Rb125Fd125))
                {
                    if (millis() - timeoutStart > 10000)
                    {
                        Serial.println("[RFM23] SET FSK MODULATION FAILED");
                        return false;
                    }
                }
                rfm23.sleep();

                Serial.println("[RFM23] INIT SUCCESS");
                rfm23.setModeIdle();
                return true;
            }

            void RFM23::reset()
            {
                Threads::Scope lock(spi1_mtx);
                rfm23.reset();
            }

            void RFM23::send(PacketComm &packet)
            {
                digitalWrite(RFM23_RX_ON, LOW);
                digitalWrite(RFM23_TX_ON, LOW);

                packet.wrapped.resize(0);
                packet.Wrap();

                Threads::Scope lock(spi1_mtx);
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
            }

            bool RFM23::recv(PacketComm &packet)
            {
                int32_t iretn = 0;

                digitalWrite(RFM23_RX_ON, LOW);
                digitalWrite(RFM23_TX_ON, HIGH);

                Threads::Scope lock(spi1_mtx);
                int wait_time = 5000 - rfm23_queue.size() * 1000;
                if (wait_time < 100)
                    wait_time = 100;
                if (rfm23.waitAvailableTimeout(wait_time))
                {
                    packet.wrapped.resize(RH_RF22_MAX_MESSAGE_LEN);
                    uint8_t bytes_recieved = packet.wrapped.size();
                    if (rfm23.recv(packet.wrapped.data(), &bytes_recieved))
                    {
                        packet.wrapped.resize(bytes_recieved);
                        iretn = packet.Unwrap();
                        rfm23.setModeIdle();

                        if (iretn < 0)
                            return false;

                        return true;
                    }
                }
                rfm23.setModeIdle();
                return false;
            }
        }
    }
}