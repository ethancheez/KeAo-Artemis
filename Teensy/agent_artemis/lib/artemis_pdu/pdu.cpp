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
            std::string state = _enable ? "ENABLE" : "DISABLE";
            std::string cmd;

            switch (_cmd)
            {
            case PDU_CMD::SW_3V3_1:
                cmd = "CMD: SW_3V3_1 ";
                break;
            case PDU_CMD::SW_3V3_2:
                cmd = "CMD: SW_3V3_2 ";
                break;
            case PDU_CMD::SW_5V_1:
                cmd = "CMD: SW_5V_1 ";
                break;
            case PDU_CMD::SW_5V_2:
                cmd = "CMD: SW_5V_2 ";
                break;
            case PDU_CMD::SW_5V_3:
                cmd = "CMD: SW_5V_3 ";
                break;
            case PDU_CMD::SW_5V_4:
                cmd = "CMD: SW_5V_4 ";
                break;
            case PDU_CMD::SW_12V:
                cmd = "CMD: SW_12V ";
                break;
            case PDU_CMD::VBATT:
                cmd = "CMD: VBATT ";
                break;
            case PDU_CMD::WDT:
                cmd = "CMD: WDT ";
                break;
            case PDU_CMD::HBRIDGE1:
                cmd = "CMD: HBRIDGE1 ";
                break;
            case PDU_CMD::HBRIDGE2:
                cmd = "CMD: HBRIDGE2 ";
                break;
            case PDU_CMD::BURN:
                cmd = "CMD: BURN ";
            default:
                break;
            }
            PDU_SEND((cmd + state).c_str());

            unsigned long timeoutStart = millis();
            while (!PDU_RECV())
            {
                if (millis() - timeoutStart > 5000)
                {
                    Serial.println("FAIL TO SEND CMD TO PDU");
                    break;
                }
            }
        }
    }
}