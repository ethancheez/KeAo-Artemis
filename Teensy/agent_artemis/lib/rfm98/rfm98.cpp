#include "rfm98.h"

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            RFM98::RFM98(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm98(slaveSelectPin, interruptPin, spi) {}

            bool RFM98::init()
            {
                Threads::Scope lock(spi1_mtx);
                SPI1.setMISO(RFM98_SPI_MISO);
                SPI1.setMOSI(RFM98_SPI_MOSI);
                SPI1.setSCK(RFM98_SPI_SCK);

                pinMode(RFM98_RST_PIN, OUTPUT);
                digitalWrite(RFM98_RST_PIN, LOW);

                reset();

                unsigned long timeoutStart = millis();
                while (!rfm98.init())
                {
                    if (millis() - timeoutStart > 10000)
                    {
                        Serial.println("[RFM98] INIT FAILED");
                        return false;
                    }
                }

                rfm98.setFrequency(RFM98_FREQ);
                rfm98.setTxPower(RFM98_TX_POWER, false);

                Serial.println("[RFM98] INIT SUCCESS");
                rfm98.sleep();
                rfm98.setModeIdle();
                return true;
            }

            void RFM98::reset()
            {
                digitalWrite(RFM98_RST_PIN, LOW);
                threads.delay(10);
                digitalWrite(RFM98_RST_PIN, HIGH);
                threads.delay(10);
            }

            void RFM98::send(const uint8_t *msg, size_t length)
            {
                Threads::Scope lock(spi1_mtx);
                rfm98.setModeTx();
                rfm98.send(msg, length);
                rfm98.waitPacketSent();
                rfm98.sleep();
                rfm98.setModeIdle();
                Serial.print("[RFM98] SENDING: [");
                for (size_t i = 0; i < length; i++)
                {
                    Serial.print(msg[i]);
                }
                Serial.println("]");
            }

            bool RFM98::recv(PacketComm *packet)
            {
                int32_t iretn = 0;
                uint8_t bytes_received = 0;

                Threads::Scope lock(spi1_mtx);
                rfm98.setModeRx();
                if (rfm98.waitAvailableTimeout(100))
                {
                    packet->wrapped.resize(RH_RF95_MAX_MESSAGE_LEN);
                    if (rfm98.recv(packet->wrapped.data(), &bytes_received))
                    {
                        packet->wrapped.resize(bytes_received);
                        packet->Unwrap();
                        rfm98.sleep();
                        rfm98.setModeIdle();

                        if (iretn < 0)
                        {
                            return false;
                        }

                        return true;
                    }
                }
                rfm98.sleep();
                rfm98.setModeIdle();
                return false;
            }
        }
    }
}
