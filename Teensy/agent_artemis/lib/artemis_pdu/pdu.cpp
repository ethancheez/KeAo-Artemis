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

        void PDU::PDU_SEND(const char *cmd)
        {
            Serial1.print(cmd);
            Serial1.print('\n');
            Serial.print("SENDING TO PDU: [");
            Serial.print(cmd);
            Serial.println(']');
        }

        bool PDU::PDU_RECV()
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

        void PDU::PDU_SWITCH(PDU_CMD _cmd, bool _enable)
        {
            const char *state = _enable ? "ENABLE" : "DISABLE";
            char *cmd = (char *)malloc(sizeof(char));
            cmd[0] = '\0';

            switch (_cmd)
            {
            case SW_3V3_1:
                strcat(cmd, "CMD: SW_3V3_1 ");
                break;
            case SW_3V3_2:
                strcat(cmd, "CMD: SW_3V3_2 ");
                break;
            case SW_5V_1:
                strcat(cmd, "CMD: SW_5V_1 ");
                break;
            case SW_5V_2:
                strcat(cmd, "CMD: SW_5V_2 ");
                break;
            case SW_5V_3:
                strcat(cmd, "CMD: SW_5V_3 ");
                break;
            case SW_5V_4:
                strcat(cmd, "CMD: SW_5V_4 ");
                break;
            case SW_12V:
                strcat(cmd, "CMD: SW_12V ");
                break;
            case VBATT:
                strcat(cmd, "CMD: VBATT ");
                break;
            case WDT:
                strcat(cmd, "CMD: WDT ");
                break;
            case HBRIDGE1:
                strcat(cmd, "CMD: HBRIDGE1 ");
                break;
            case HBRIDGE2:
                strcat(cmd, "CMD: HBRIDGE2 ");
                break;
            case BURN:
                strcat(cmd, "CMD: BURN ");
            }
            PDU::PDU_SEND(strcat(cmd, state));

            unsigned long timeoutStart = millis();
            while (!PDU::PDU_RECV())
            {
                if (millis() - timeoutStart > 5000)
                {
                    Serial.println("FAIL TO SEND CMD TO PDU");
                    break;
                }
            }

            free(cmd);
        }
    }
}