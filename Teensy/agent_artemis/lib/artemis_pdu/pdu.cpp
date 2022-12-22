#include <pdu.h>

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
            memcpy(cmd, &packet, sizeof(packet));

            std::string msg = "";
            for (size_t i = 0; i < sizeof(packet); i++)
            {
                msg += cmd[i] + PDU_CMD_OFFSET;
            }
            Serial1.print(msg.c_str());
            Serial1.print('\n');

            Serial.print("SENDING TO PDU: [");
            for (size_t i = 0; i < msg.length(); i++)
            {
                Serial.print(msg[i] - PDU_CMD_OFFSET, HEX);
            }
            Serial.println(']');

            free(cmd);
        }

        int PDU::recv(std::string &response)
        {
            if (Serial1.available() > 0)
            {
                String UART1_RX = Serial1.readString();
                if (UART1_RX.length() > 0)
                {
                    // Serial.print("UART RECV: ");
                    // Serial.println(UART1_RX);
                    response = UART1_RX.c_str();
                    return UART1_RX.length();
                }
            }
            return -1;
        }

        void PDU::set_switch(PDU_SW sw, uint8_t _enable)
        {
            std::string response;
            pdu_packet packet;
            packet.type = PDU_Type::CommandSetSwitch;
            packet.sw = sw;
            packet.sw_state = _enable > 0 ? 1 : 0;

            unsigned long timeoutStart = millis();
            while (1)
            {
                send(packet);
                while (recv(response) < 0)
                {
                    if (millis() - timeoutStart > 5000)
                    {
                        Serial.println("FAIL TO SEND CMD TO PDU");
                        break;
                    }
                }
                if ((response[1] == (uint8_t)sw + PDU_CMD_OFFSET) || (sw == PDU_SW::All && response[0] == (uint8_t)PDU::PDU_Type::DataSwitchTelem + PDU_CMD_OFFSET))
                {
                    break;
                }

                delay(100);
            }
            // TODO: Send to PacketComm packet -> then to ground
            Serial.print("UART RECV: ");
            Serial.println(response.c_str());
        }

        bool PDU::get_switch(PDU_SW sw)
        {
            std::string response;
            pdu_packet packet;
            packet.type = PDU_Type::CommandGetSwitchStatus;
            packet.sw = sw;

            unsigned long timeoutStart = millis();
            while (1)
            {
                send(packet);
                while (recv(response) < 0)
                {
                    if (millis() - timeoutStart > 5000)
                    {
                        Serial.println("FAIL TO SEND CMD TO PDU");
                        break;
                    }
                }
                if ((response[1] == (uint8_t)sw + PDU_CMD_OFFSET) || (sw == PDU_SW::All && response[0] == (uint8_t)PDU::PDU_Type::DataSwitchTelem + PDU_CMD_OFFSET))
                {
                    break;
                }
                delay(100);
            }
            // TODO: Send to PacketComm packet -> then to ground
            Serial.print("UART RECV: ");
            Serial.println(response.c_str());

            // TODO: Return true when on, false when off
            return true;
        }
    }
}