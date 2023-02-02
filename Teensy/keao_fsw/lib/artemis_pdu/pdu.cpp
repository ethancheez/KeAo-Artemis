#include <pdu.h>

namespace Artemis
{
    namespace Teensy
    {
        PDU::PDU(HardwareSerial *hw_serial, int baud_rate)
        {
            serial = hw_serial;
            serial->begin(baud_rate);
        }

        int32_t PDU::send(pdu_packet packet)
        {
            char *cmd = (char *)malloc(sizeof(packet));
            memcpy(cmd, &packet, sizeof(packet));

            std::string msg = "";
            for (size_t i = 0; i < sizeof(packet); i++)
            {
                msg += cmd[i] + PDU_CMD_OFFSET;
            }
            serial->print(msg.c_str());
            serial->print('\n');

            Serial.print("SENDING TO PDU: [");
            for (size_t i = 0; i < msg.length(); i++)
            {
                Serial.print(msg[i] - PDU_CMD_OFFSET, HEX);
            }
            Serial.println(']');

            free(cmd);
            return 0;
        }

        int32_t PDU::recv(std::string &response)
        {
            if (serial->available() > 0)
            {
                String UART1_RX = serial->readString();
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

        int32_t PDU::set_switch(PDU_SW sw, uint8_t _enable)
        {
            std::string response;
            pdu_packet packet;
            packet.type = PDU_Type::CommandSetSwitch;
            packet.sw = sw;
            packet.sw_state = _enable > 0 ? 1 : 0;

            uint8_t attempts = 1;
            elapsedMillis timeout;
            while (1)
            {
                send(packet);
                while (recv(response) < 0)
                {
                    if (timeout > 5000)
                    {
                        Serial.print("Attempt ");
                        Serial.print(attempts);
                        Serial.println(": FAIL TO SEND CMD TO PDU");
                        timeout = 0;

                        if (++attempts == 5)
                        {
                            return -1;
                        }
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
            return 0;
        }

        int32_t PDU::get_switch(PDU_SW sw)
        {
            std::string response;
            pdu_packet packet;
            packet.type = PDU_Type::CommandGetSwitchStatus;
            packet.sw = sw;

            uint8_t attempts = 1;
            elapsedMillis timeout;
            while (1)
            {
                send(packet);
                while (recv(response) < 0)
                {
                    if (timeout > 5000)
                    {
                        Serial.print("Attempt ");
                        Serial.print(attempts);
                        Serial.println(": FAIL TO SEND CMD TO PDU");
                        timeout = 0;

                        if (++attempts == 5)
                        {
                            return -1;
                        }
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

            if (sw == PDU_SW::All)
            {
                for (size_t i = 2; i < response.length(); i++)
                {
                    if (response[i] == '0')
                    {
                        return 0;
                    }
                }
                return 1;
            }

            return response[3] - PDU_CMD_OFFSET;
        }
    }
}