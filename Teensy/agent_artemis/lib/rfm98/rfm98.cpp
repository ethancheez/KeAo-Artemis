#include "rfm98.h"

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            RFM98::RFM98(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm98(slaveSelectPin, interruptPin, spi) {}

            void RFM98::RFM98_INIT()
            {
                Threads::Scope scope(spi1_mtx);
                SPI1.setMISO(RFM98_SPI_MISO);
                SPI1.setMOSI(RFM98_SPI_MOSI);
                SPI1.setSCK(RFM98_SPI_SCK);

                pinMode(RFM98_RST_PIN, OUTPUT);
                digitalWrite(RFM98_RST_PIN, LOW);

                RFM98_RESET();

                while (!(rfm98.init()))
                {
                    Serial.println("RFM98 INIT FAILED");
                }

                rfm98.setFrequency(RFM98_FREQ);
                rfm98.setTxPower(RFM98_TX_POWER, false);

                Serial.println("RFM98 successfully initialized!");
            }

            void RFM98::RFM98_RESET()
            {
                digitalWrite(RFM98_RST_PIN, LOW);
                delay(10);
                digitalWrite(RFM98_RST_PIN, HIGH);
                delay(10);
            }

            void RFM98::RFM98_SEND(const unsigned char *msg, size_t length)
            {
                Serial.print("[RFM98] Sending: [");
                for (size_t i = 0; i < length; ++i)
                {
                    Serial.print(*(msg + i));
                    Serial.print(" ");
                }
                Serial.println("]");

                Threads::Scope scope(spi1_mtx);
                rfm98.send((uint8_t *)msg, length);

                rfm98.waitPacketSent();
            }

            void RFM98::RFM98_RECV()
            {
                uint8_t bytes_received = 0;

                Threads::Scope scope(spi1_mtx);
                if (rfm98.waitAvailableTimeout(100))
                {
                    packet.packetized.resize(RFM98_RECV_LEN);
                    if (rfm98.recv(packet.packetized.data(), &bytes_received))
                    {
                        Serial.print("[RFM98] Reply: [");
                        for (int i = 0; i < RFM98_RECV_LEN; i++)
                        {
                            Serial.print((char)packet.packetized[i]);
                        }
                        Serial.println("]");
                        Serial.print("RSSI: ");
                        Serial.println(rfm98.lastRssi(), DEC);
                        // send packet to the main queue
                        packet.packetized.resize(bytes_received);
                        packet.RawUnPacketize();
                        main_queue.push(packet);
                        packet.packetized.resize(RFM98_RECV_LEN, 0);
                    }
                    else
                    {
                        Serial.println("Receive failed");
                    }
                }
            }
        }
    }
}
