#ifndef _PDU_H
#define _PDU_H

#include <stdint.h>
#include <map>
#include <string>

namespace Artemis
{
    namespace Teensy
    {
        class PDU
        {
        public:
            enum class PDU_CMD
            {
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
                RPI,
            };

            std::map<std::string, PDU_CMD> PDUType = {
                {"3v3_1", PDU_CMD::SW_3V3_1},
                {"3v3_2", PDU_CMD::SW_3V3_2},
                {"5v_1", PDU_CMD::SW_5V_1},
                {"5v_2", PDU_CMD::SW_5V_2},
                {"5v_3", PDU_CMD::SW_5V_3},
                {"5v_4", PDU_CMD::SW_5V_4},
                {"12v", PDU_CMD::SW_12V},
                {"vbatt", PDU_CMD::VBATT},
                {"wdt", PDU_CMD::WDT},
                {"hbridge1", PDU_CMD::HBRIDGE1},
                {"hbridge2", PDU_CMD::HBRIDGE2},
                {"burn", PDU_CMD::BURN},
                {"rpi", PDU_CMD::RPI},
            };

            PDU(int baud_rate);
            void PDU_INIT();
            void PDU_SWITCH(PDU_CMD cmd, bool enable);
            void PDU_SEND(const char *cmd);
            bool PDU_RECV();
        };
    }
}

#endif
