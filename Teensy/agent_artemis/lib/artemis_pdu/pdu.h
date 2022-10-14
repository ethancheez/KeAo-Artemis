#include <stdint.h>

namespace Artemis
{
    namespace Teensy
    {
        class PDU
        {
        public:
            enum PDU_CMD
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
            };

            PDU(int baud_rate);
            void PDU_INIT();
            void PDU_SWITCH(PDU_CMD cmd, bool enable);
            void PDU_SEND(const char *cmd);
            bool PDU_RECV();
        };
    }
}