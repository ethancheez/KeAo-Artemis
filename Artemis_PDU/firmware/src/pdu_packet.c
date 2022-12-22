#include "pdu_packet.h"
#include "definitions.h"

void decode_pdu_packet(const char *input)
{
    struct pdu_packet packet;
    packet.type = input[0] - PDU_CMD_OFFSET;
    packet.sw = input[1] - PDU_CMD_OFFSET;
    packet.sw_state = input[2] - PDU_CMD_OFFSET;
    
    char *reply = malloc(sizeof(struct pdu_packet));
    switch(packet.type)
    {
        case CommandPing:
            packet.type = DataPong + PDU_CMD_OFFSET;
            packet.sw += PDU_CMD_OFFSET;
            packet.sw_state += PDU_CMD_OFFSET;
            memcpy(reply, &packet, sizeof(struct pdu_packet));
            SERCOM3_USART_Write(&reply[0], sizeof(reply));
            SERCOM3_USART_Write("\r\n", 2);
            break;
        case CommandSetSwitch:
            if(packet.sw_state == 1)
            {
                switch(packet.sw)
                {
                    case All:
                        enableAllGPIOs();
                        break;
                    case SW_3V3_1:
                        SW_3V3_EN1_Set();
                        break;
                    case SW_3V3_2:
                        SW_3V3_EN2_Set();
                        break;
                    case SW_5V_1:
                        SW_5V_EN1_Set();
                        break;
                    case SW_5V_2:
                        SW_5V_EN2_Set();
                        break;
                    case SW_5V_3:
                        SW_5V_EN3_Set();
                        break;
                    case SW_5V_4:
                        SW_5V_EN4_Set();
                        break;
                    case SW_12V:
                        SW_12V_EN1_Set();
                        SW_5V_EN4_Set();
                        break;
                    case VBATT:
                        SW_VBATT_EN_Set();
                        break;
                    case WDT:
                        WDT_WDI_Set();
                        break;
                    case HBRIDGE1:
                        FAULT1_Set();
                        IN1_Set();
                        IN2_Set();
                        IN3_Set();
                        IN4_Set();
                        TRQ1_Set();
                        SLEEP1_Set();
                        break;
                    case HBRIDGE2:
                        FAULT2_Set();
                        IN5_Set();
                        IN6_Set();
                        IN7_Set();
                        IN8_Set();
                        TRQ2_Set();
                        SLEEP2_Set();
                        break;
                    case BURN:
                        BURN1_EN_Set();
                        BURN2_EN_Set();
                        BURN_5V_Set();
                        break;
                    case BURN1:
                        BURN1_EN_Set();
                        BURN_5V_Set();
                        break;
                    case BURN2:
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
                    case All:
                        disableAllGPIOs();
                        break;
                    case SW_3V3_1:
                        SW_3V3_EN1_Clear();
                        break;
                    case SW_3V3_2:
                        SW_3V3_EN2_Clear();
                        break;
                    case SW_5V_1:
                        SW_5V_EN1_Clear();
                        break;
                    case SW_5V_2:
                        SW_5V_EN2_Clear();
                        break;
                    case SW_5V_3:
                        SW_5V_EN3_Clear();
                        break;
                    case SW_5V_4:
                        SW_5V_EN4_Clear();
                        break;
                    case SW_12V:
                        SW_12V_EN1_Clear();
                        SW_5V_EN4_Clear();
                        break;
                    case VBATT:
                        SW_VBATT_EN_Clear();
                        break;
                    case WDT:
                        WDT_WDI_Clear();
                        break;
                    case HBRIDGE1:
                        FAULT1_Clear();
                        IN1_Clear();
                        IN2_Clear();
                        IN3_Clear();
                        IN4_Clear();
                        TRQ1_Clear();
                        SLEEP1_Clear();
                        break;
                    case HBRIDGE2:
                        FAULT2_Clear();
                        IN5_Clear();
                        IN6_Clear();
                        IN7_Clear();
                        IN8_Clear();
                        TRQ2_Clear();
                        SLEEP2_Clear();
                        break;
                    case BURN:
                        BURN1_EN_Clear();
                        BURN2_EN_Clear();
                        BURN_5V_Clear();
                        break;
                    case BURN1:
                        if(!PORT_PinRead(BURN2_EN_PIN))
                            BURN_5V_Clear();
                        BURN1_EN_Clear();
                        break;
                    case BURN2:
                        if(!PORT_PinRead(BURN1_EN_PIN))
                            BURN_5V_Clear();
                        BURN2_EN_Clear();
                        break;
                    default:
                        break;
                }
            }
        case CommandGetSwitchStatus:
            if(packet.sw == All) {
                free(reply);
                reply = malloc(sizeof(struct pdu_telem));
                struct pdu_telem telem;
                telem.type = DataSwitchTelem + PDU_CMD_OFFSET;
                telem.sw_state[0] = PORT_PinRead(SW_3V3_EN1_PIN) + PDU_CMD_OFFSET;
                telem.sw_state[1] = PORT_PinRead(SW_3V3_EN2_PIN)+ PDU_CMD_OFFSET;
                telem.sw_state[2] = PORT_PinRead(SW_5V_EN1_PIN) + PDU_CMD_OFFSET;
                telem.sw_state[3] = PORT_PinRead(SW_5V_EN2_PIN) + PDU_CMD_OFFSET;
                telem.sw_state[4] = PORT_PinRead(SW_5V_EN3_PIN) + PDU_CMD_OFFSET;
                telem.sw_state[5] = PORT_PinRead(SW_5V_EN4_PIN) + PDU_CMD_OFFSET;
                telem.sw_state[6] = (PORT_PinRead(SW_12V_EN1_PIN) && PORT_PinRead(SW_5V_EN4_PIN)) + PDU_CMD_OFFSET;
                telem.sw_state[7] = PORT_PinRead(SW_VBATT_EN_PIN) + PDU_CMD_OFFSET;
                telem.sw_state[8] = (PORT_PinRead(BURN1_EN_PIN) && PORT_PinRead(BURN_5V_PIN)) + PDU_CMD_OFFSET;
                telem.sw_state[9] = (PORT_PinRead(BURN2_EN_PIN) && PORT_PinRead(BURN_5V_PIN)) + PDU_CMD_OFFSET;
                telem.sw_state[10] = (PORT_PinRead(FAULT1_PIN) &&
                                PORT_PinRead(IN1_PIN) &&
                                PORT_PinRead(IN2_PIN) &&
                                PORT_PinRead(IN3_PIN) &&
                                PORT_PinRead(IN4_PIN) &&
                                PORT_PinRead(TRQ1_PIN) &&
                                PORT_PinRead(SLEEP1_PIN)) + PDU_CMD_OFFSET;
                telem.sw_state[11] = (PORT_PinRead(FAULT2_PIN) &&
                                PORT_PinRead(IN5_PIN) &&
                                PORT_PinRead(IN6_PIN) &&
                                PORT_PinRead(IN7_PIN) &&
                                PORT_PinRead(IN8_PIN) &&
                                PORT_PinRead(TRQ2_PIN) &&
                                PORT_PinRead(SLEEP2_PIN)) + PDU_CMD_OFFSET;
                memcpy(reply, &telem, sizeof(struct pdu_telem));
                SERCOM3_USART_Write(&reply[0], sizeof(struct pdu_telem));
                SERCOM3_USART_Write("\r\n", 2);
                break;
            } 
            packet.type = DataSwitchStatus + PDU_CMD_OFFSET;
            switch(packet.sw)
            {
                case SW_3V3_1:
                    packet.sw_state = PORT_PinRead(SW_3V3_EN1_PIN);
                    break;
                case SW_3V3_2:
                    packet.sw_state = PORT_PinRead(SW_3V3_EN2_PIN);
                    break;
                case SW_5V_1:
                    packet.sw_state = PORT_PinRead(SW_5V_EN1_PIN);
                    break;
                case SW_5V_2:
                    packet.sw_state = PORT_PinRead(SW_5V_EN2_PIN);
                    break;
                case SW_5V_3:
                    packet.sw_state = PORT_PinRead(SW_5V_EN3_PIN);
                    break;
                case SW_5V_4:
                    packet.sw_state = PORT_PinRead(SW_5V_EN4_PIN);
                    break;
                case SW_12V:
                    packet.sw_state = PORT_PinRead(SW_12V_EN1_PIN) && PORT_PinRead(SW_5V_EN4_PIN);
                    break;
                case VBATT:
                    packet.sw_state = PORT_PinRead(SW_VBATT_EN_PIN);
                    break;
                case BURN1:
                    packet.sw_state = PORT_PinRead(BURN1_EN_PIN) && PORT_PinRead(BURN_5V_PIN);
                    break;
                case BURN2:
                    packet.sw_state = PORT_PinRead(BURN2_EN_PIN) && PORT_PinRead(BURN_5V_PIN);
                    break;
                case HBRIDGE1:
                    packet.sw_state = PORT_PinRead(FAULT1_PIN) &&
                            PORT_PinRead(IN1_PIN) &&
                            PORT_PinRead(IN2_PIN) &&
                            PORT_PinRead(IN3_PIN) &&
                            PORT_PinRead(IN4_PIN) &&
                            PORT_PinRead(TRQ1_PIN) &&
                            PORT_PinRead(SLEEP1_PIN);
                    break;
                case HBRIDGE2:
                    packet.sw_state = PORT_PinRead(FAULT2_PIN) &&
                            PORT_PinRead(IN5_PIN) &&
                            PORT_PinRead(IN6_PIN) &&
                            PORT_PinRead(IN7_PIN) &&
                            PORT_PinRead(IN8_PIN) &&
                            PORT_PinRead(TRQ2_PIN) &&
                            PORT_PinRead(SLEEP2_PIN);
                    break;
                default:
                    break;
            }
            packet.sw += PDU_CMD_OFFSET;
            packet.sw_state += PDU_CMD_OFFSET;
            memcpy(reply, &packet, sizeof(struct pdu_packet));
            SERCOM3_USART_Write(&reply[0], sizeof(reply));
            SERCOM3_USART_Write("\r\n", 2);
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
//    WDT_WDI_Set();
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
//    WDT_WDI_Clear();
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
