#include "rfm23.h"

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            RFM23::RFM23(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi) : rfm23(slaveSelectPin, interruptPin, spi) {}

            void RFM23::RFM23_INIT()
            {
                SPI1.setMISO(RFM23_SPI_MISO);
                SPI1.setMOSI(RFM23_SPI_MOSI);
                SPI1.setSCK(RFM23_SPI_SCK);
                pinMode(RFM23_RX_ON, OUTPUT);
                pinMode(RFM23_TX_ON, OUTPUT);

                // while loop to give multiple init attempts
                while (!rfm23.init())
                {
                    Serial.print("driver init failed: ");
                    Serial.println();


                    delay(5000);
                }
                rfm23.setFrequency(430); // frequency default is 434MHz
                rfm23.setTxPower(20);    // 20 is the max
            }

            void RFM23::RFM23_RESET()
            {
                rfm23.reset();
            }

            void RFM23::RFM23_SEND(const char *msg)
            {
                digitalWrite(RFM23_RX_ON, HIGH);
                digitalWrite(RFM23_TX_ON, LOW);
                Serial.print("[RFM23] Sending: [");
                Serial.print(msg);
                Serial.println("]");

                rfm23.send((uint8_t *)msg, strlen(msg));

                rfm23.waitPacketSent();
            }

            void RFM23::RFM23_RECV()
            {

                digitalWrite(RFM23_RX_ON, LOW);
                digitalWrite(RFM23_TX_ON, HIGH);

                if (rfm23.waitAvailableTimeout(100))
                {
                    if (rfm23.recv(RFM23_RECV_BUF, &RFM23_RECV_LEN))
                    {
                        Serial.print("[RFM23] Reply: [");
                        for (int i = 0; i < RFM23_RECV_LEN; i++)
                        {
                            Serial.print((char)RFM23_RECV_BUF[i]);
                        }
                        Serial.println("]");
                        Serial.print("RSSI: ");
                        Serial.println(rfm23.lastRssi(), DEC);
                        memset(RFM23_RECV_BUF, '\0', RH_RF22_MAX_MESSAGE_LEN);
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