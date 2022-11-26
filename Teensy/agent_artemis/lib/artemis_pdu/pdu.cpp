#include <pdu.h>
#include <Arduino.h>

namespace Artemis
{
    namespace Teensy
    {
        PDU::PDU(int baud_rate)
        {
            Serial1.begin(baud_rate);
        }

        void PDU::send(pdu_packet packet)
        {
            char *cmd = (char *)malloc(sizeof(packet));
            memcpy(&cmd, &packet, sizeof(packet));

            Serial1.print(cmd);
            Serial1.print('\n');
            Serial.print("SENDING TO PDU: [");
            Serial.print(cmd);
            Serial.println(']');

            free(cmd);
        }

        bool PDU::recv()
        {
            if (Serial1.available() > 0)
            {
                String UART1_RX = Serial1.readString();
                if (UART1_RX.length() > 0)
                {
                    Serial.print("UART RECV: ");
                    Serial.println(UART1_RX);
                    return true;
                }
            }
            return false;
        }

        void PDU::set_switch(PDU_SW sw, uint8_t _enable)
        {
            pdu_packet packet;
            packet.type = PDU_Type::CommandSetSwitch;
            packet.sw = sw;
            packet.sw_state = _enable > 0 ? 1 : 0;

            send(packet);

            unsigned long timeoutStart = millis();
            while (!recv())
            {
                if (millis() - timeoutStart > 5000)
                {
                    Serial.println("FAIL TO SEND CMD TO PDU");
                    break;
                }
            }
        }

        bool PDU::get_switch(PDU_SW sw)
        {
            pdu_packet packet;
            packet.type = PDU_Type::CommandGetSwitchStatus;
            packet.sw = sw;

            send(packet);

            unsigned long timeoutStart = millis();
            while (!recv())
            {
                if (millis() - timeoutStart > 5000)
                {
                    Serial.println("FAIL TO SEND CMD TO PDU");
                    break;
                }
            }

            return true;
        }
    }
}