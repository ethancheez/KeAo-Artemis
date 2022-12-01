#ifndef _PDU_PACKET_H    /* Guard against multiple inclusion */
#define _PDU_PACKET_H

#include "string.h"
#include "stdint.h"

enum PDU_Type
{
    NOP,
    CommandPing,
    CommandSetSwitch,
    CommandGetSwitchStatus,
    DataPong,
    DataSwitchStatus,
};
typedef uint8_t PDU_Type;

enum PDU_SW
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
typedef uint8_t PDU_SW;

struct __attribute__((packed)) pdu_packet
{
    PDU_Type type;
    PDU_SW sw;
    uint8_t sw_state;
};

void decode_pdu_packet(const uint8_t *input);
void enableAllGPIOs(void);
void disableAllGPIOs(void);

#endif /* _PDU_PACKET_H */

/* *****************************************************************************
 End of File
 */
