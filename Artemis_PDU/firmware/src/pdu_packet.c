#include "pdu_packet.h"
#include "definitions.h"

char newline[] = "\r\n";

void decode_pdu_packet(const char *input)
{
    pdu_packet packet;
    memcpy(&packet, &input, sizeof(input));
    
    const char *reply = (char *)malloc(sizeof(packet));
    switch(packet.type)packet
    {
        case PDU_Type::CommandPing:
            packet.type = PDU_Type::DataPong;
            memcpy(reply, &packet, sizeof(packet));
            SERCOM3_USART_Write(&reply, sizeof(reply));
            SERCOM3_USART_Write(&newline[0],sizeof(newline));
            break;
        case PDU_Type::CommandSetSwitch:
            if(packet.sw_state == 1)
            {
                switch(packet.sw)
                {
                    case PDU_SW::All:
                        enableAllGPIOs();
                        break;
                    case PDU_SW::SW_3V3_1:
                        SW_3V3_EN1_Set();
                        break;
                    case PDU_SW::SW_3V3_2:
                        SW_3V3_EN2_Set();
                        break;
                    case PDU_SW::SW_5V_1:
                        SW_5V_EN1_Set();
                        break;
                    case PDU_SW::SW_5V_2:
                        SW_5V_EN2_Set();
                        break;
                    case PDU_SW::SW_5V_3:
                        SW_5V_EN3_Set();
                        break;
                    case PDU_SW::SW_5V_4:
                        SW_5V_EN4_Set();
                        break;
                    case PDU_SW::SW_12V:
                        SW_12V_EN1_Set();
                        SW_5V_EN4_Set();
                        break;
                    case PDU_SW::VBATT:
                        SW_VBATT_EN_Set();
                        break;
                    case PDU_SW::WDT:
                        WDT_WDI_Set();
                        break;
                    case PDU_SW::HBRIDGE1:
                        FAULT1_Set();
                        IN1_Set();
                        IN2_Set();
                        IN3_Set();
                        IN4_Set();
                        TRQ1_Set();
                        SLEEP1_Set();
                        break;
                    case PDU_SW::HBRIDGE2:
                        FAULT2_Set();
                        IN5_Set();
                        IN6_Set();
                        IN7_Set();
                        IN8_Set();
                        TRQ2_Set();
                        SLEEP2_Set();
                        break;
                    case PDU_SW::BURN:
                        BURN1_EN_Set();
                        BURN2_EN_Set();
                        BURN_5V_Set();
                        break;
                    case PDU_SW::BURN1:
                        BURN1_EN_Set();
                        BURN_5V_Set();
                        break;
                    case PDU_SW::BURN2:
                        BURN2_EN_Set();
                        BURN_5V_Set();
                        break;
                    default:
                        break;
                }
            } else
            {
                switch(packet.sw)
                {
                    case PDU_SW::All:
                        disableAllGPIOs();
                        break;
                    case PDU_SW::SW_3V3_1:
                        SW_3V3_EN1_Clear();
                        break;
                    case PDU_SW::SW_3V3_2:
                        SW_3V3_EN2_Clear();
                        break;
                    case PDU_SW::SW_5V_1:
                        SW_5V_EN1_Clear();
                        break;
                    case PDU_SW::SW_5V_2:
                        SW_5V_EN2_Clear();
                        break;
                    case PDU_SW::SW_5V_3:
                        SW_5V_EN3_Clear();
                        break;
                    case PDU_SW::SW_5V_4:
                        SW_5V_EN4_Clear();
                        break;
                    case PDU_SW::SW_12V:
                        SW_12V_EN1_Clear();
                        SW_5V_EN4_Clear();
                        break;
                    case PDU_SW::VBATT:
                        SW_VBATT_EN_Clear();
                        break;
                    case PDU_SW::WDT:
                        WDT_WDI_Clear();
                        break;
                    case PDU_SW::HBRIDGE1:
                        FAULT1_Clear();
                        IN1_Clear();
                        IN2_Clear();
                        IN3_Clear();
                        IN4_Clear();
                        TRQ1_Clear();
                        SLEEP1_Clear();
                        break;
                    case PDU_SW::HBRIDGE2:
                        FAULT2_Clear();
                        IN5_Clear();
                        IN6_Clear();
                        IN7_Clear();
                        IN8_Clear();
                        TRQ2_Clear();
                        SLEEP2_Clear();
                        break;
                    case PDU_SW::BURN:
                        BURN1_EN_Clear();
                        BURN2_EN_Clear();
                        BURN_5V_Clear();
                        break;
                    case PDU_SW::BURN1:
                        BURN1_EN_Clear();
                        break;
                    case PDU_SW::BURN2:
                        BURN2_EN_Clear();
                        break;
                    default:
                        break;
                }
            }
        case PDU_Type::CommandGetSwitchStatus:
            packet.type = PDU_Type::DataSwitchStatus;
            switch(packet.sw)
            {
                case PDU_SW::SW_3V3_1:
                    packet.sw_state = SW_3V3_EN1_Get();
                    break;
                case PDU_SW::SW_3V3_2:
                    packet.sw_state = SW_3V3_EN2_Get();
                    break;
                case PDU_SW::SW_5V_1:
                    packet.sw_state = SW_5V_EN1_Get();
                    break;
                case PDU_SW::SW_5V_2:
                    packet.sw_state = SW_5V_EN2_Get();
                    break;
                case PDU_SW::SW_5V_3:
                    packet.sw_state = SW_5V_EN3_Get();
                    break;
                case PDU_SW::SW_5V_4:
                    packet.sw_state = SW_5V_EN4_Get();
                    break;
                case PDU_SW::SW_12V:
                    packet.sw_state = SW_12V_EN1_Get() && SW_5V_EN4_Get();
                    break;
                case PDU_SW::VBATT:
                    packet.sw_state = SW_VBATT_EN_Get();
                    break;
                case PDU_SW::BURN1:
                    packet.sw_state = BURN1_EN_Get() && BURN_5V_Get();
                    break;
                case PDU_SW::BURN2:
                    packet.sw_state = BURN2_EN_Get() && BURN_5V_Get();
                    break;
                default:
                    break;
            }
            memcpy(reply, &packet, sizeof(packet));
            SERCOM3_USART_Write(&reply, sizeof(reply));
            SERCOM3_USART_Write(&newline[0],sizeof(newline));
            break;
        default:
            break;
    };
    free(reply);
}

void enableAllGPIOs(void) {
    BURN_5V_Set();
    BURN2_EN_Set();
    SW_12V_EN1_Set();
    SW_3V3_EN1_Set();
    SW_3V3_EN2_Set();
    SW_5V_EN1_Set();
    SW_5V_EN2_Set();
    SW_5V_EN3_Set();
    SW_5V_EN4_Set();
    SW_VBATT_EN_Set();
    WDT_WDI_Set();
    BURN1_EN_Set();
    IN1_Set();
    IN2_Set();
    IN3_Set();
    IN4_Set();
    IN5_Set();
    IN6_Set();
    IN7_Set();
    IN8_Set();
    TRQ1_Set();
    TRQ2_Set();
    FAULT1_Set();
    FAULT2_Set();
    SLEEP1_Set();
    SLEEP2_Set();
}

void disableAllGPIOs(void) {
    BURN_5V_Clear();
    BURN2_EN_Clear();
    SW_12V_EN1_Clear();
    SW_3V3_EN2_Clear();
    SW_3V3_EN1_Clear();
    SW_5V_EN2_Clear();
    SW_5V_EN1_Clear();
    SW_5V_EN3_Clear();
    SW_5V_EN4_Clear();
    SW_VBATT_EN_Clear();
    WDT_WDI_Clear();
    BURN1_EN_Set();
    IN1_Clear();
    IN2_Clear();
    IN3_Clear();
    IN4_Clear();
    IN5_Clear();
    IN6_Clear();
    IN7_Clear();
    IN8_Clear();
    TRQ1_Clear();
    TRQ2_Clear();
    FAULT1_Clear();
    FAULT2_Clear();
    SLEEP1_Clear();
    SLEEP2_Clear();
}
