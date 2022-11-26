#ifndef _PDU_PACKET_H    /* Guard against multiple inclusion */
#define _PDU_PACKET_H

#include "string.h"

enum PDU_Type: uint8_t
{
    None,
    CommandPing,
    CommandSetSwitch,
    CommandGetSwitchStatus,
    DataPong,
    DataSwitchStatus,
};

enum PDU_SW : uint8_t
{
    None,
    All,
    SW_3V3_1,
    SW_3V3_2,
    SW_5V_1,
    SW_5V_2,
    SW_5V_3,
    SW_5V_4,
    SW_12V,
    VBATT,
    WDT,
    HBRIDGE1,
    HBRIDGE2,
    BURN,
    BURN1,
    BURN2,
};

struct __attribute__((packed)) pdu_packet
{
    PDU_Type type = PDU_Type::None;
    PDU_SW sw = PDU_SW::None;
    uint8_t sw_state = 0;
};

void decode_pdu_packet(const char *input);
void enableAllGPIOs(void);
void disableAllGPIOs(void);

#endif /* _PDU_PACKET_H */

/* *****************************************************************************
 End of File
 */
