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
                Threads::Scope scope(spi1_mtx);
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

                timeoutStart = millis();
                while (!rfm23.setModemConfig(RH_RF22::FSK_Rb_512Fd2_5))
                {
                    if (millis() - timeoutStart > 10000)
                    {
                        Serial.println("[RFM23] SET FSK MODULATION FAILED");
                        return false;
                    }
                }

                Serial.println("[RFM23] INIT SUCCESS");
                return true;
            }

            void RFM23::reset()
            {
                Threads::Scope scope(spi1_mtx);
                rfm23.reset();
            }

            void RFM23::send(const unsigned char *msg, size_t length)
            {
                digitalWrite(RFM23_RX_ON, HIGH);
                digitalWrite(RFM23_TX_ON, LOW);
                // Serial.print("[RFM23] Sending: [");
                // for (size_t i = 0; i < length; ++i)
                // {
                //     Serial.print(*(msg + i));
                //     Serial.print(" ");
                // }
                // Serial.println("]");

                Threads::Scope scope(spi1_mtx);
                rfm23.send((uint8_t *)msg, length);

                rfm23.waitPacketSent();
            }

            PacketComm RFM23::recv()
            {
                packet.packetized.resize(0);
                digitalWrite(RFM23_RX_ON, LOW);
                digitalWrite(RFM23_TX_ON, HIGH);
                uint8_t bytesrecieved = 0;

                Threads::Scope scope(spi1_mtx);
                if (rfm23.waitAvailableTimeout(100))
                {
                    packet.packetized.resize(RH_RF22_MAX_MESSAGE_LEN);
                    if (rfm23.recv(packet.packetized.data(), &bytesrecieved))
                    {
                        // Serial.print("[RFM23] Reply: [");
                        // for (int i = 0; i < bytesrecieved; i++)
                        // {
                        //     Serial.print((char)packet.packetized[i]);
                        // }
                        // Serial.println("]");
                        // Serial.print("RSSI: ");
                        // Serial.println(rfm23.lastRssi(), DEC);

                        packet.packetized.resize(bytesrecieved);
                        packet.RawUnPacketize();
                        // main_queue.push(packet);
                        // packet.packetized.resize(RFM23_RECV_LEN, 0);
                    }
                }

                return packet;
            }
        }
    }
}