#ifndef _PDU_H
#define _PDU_H

#include <Arduino.h>
#include <stdint.h>
#include "support/configCosmosKernel.h"

#define PDU_CMD_OFFSET 48

namespace Artemis
{
    namespace Teensy
    {
        class PDU
        {
        public:
            enum class PDU_Type : uint8_t
            {
                NOP,
                CommandPing,
                CommandSetSwitch,
                CommandGetSwitchStatus,
                DataPong,
                DataSwitchStatus,
                DataSwitchTelem,
            };

            enum class PDU_SW : uint8_t
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
                RPI,
            };

            std::map<std::string, PDU_SW> PDU_SW_Type = {
                {"all", PDU_SW::All},
                {"3v3_1", PDU_SW::SW_3V3_1},
                {"3v3_2", PDU_SW::SW_3V3_2},
                {"5v_1", PDU_SW::SW_5V_1},
                {"5v_2", PDU_SW::SW_5V_2},
                {"5v_3", PDU_SW::SW_5V_3},
                {"5v_4", PDU_SW::SW_5V_4},
                {"12v", PDU_SW::SW_12V},
                {"vbatt", PDU_SW::VBATT},
                {"wdt", PDU_SW::WDT},
                {"hbridge1", PDU_SW::HBRIDGE1},
                {"hbridge2", PDU_SW::HBRIDGE2},
                {"burn", PDU_SW::BURN},
                {"burn1", PDU_SW::BURN1},
                {"burn2", PDU_SW::BURN2},
                {"rpi", PDU_SW::RPI},
            };

            struct __attribute__((packed)) pdu_packet
            {
                PDU_Type type = PDU_Type::NOP;
                PDU_SW sw = PDU_SW::None;
                uint8_t sw_state = 0;
            };

            struct __attribute__((packed)) pdu_telem
            {
                PDU_Type type = PDU_Type::DataSwitchTelem;
                uint8_t sw_state[12];
            };

            PDU(HardwareSerial *hw_serial, int baud_rate);
            int32_t set_switch(PDU_SW sw, uint8_t enable);
            int32_t get_switch(PDU_SW sw);
            int32_t send(pdu_packet packet);
            int32_t recv(std::string &response);

        private:
            HardwareSerial *serial;
        };
    }
}

#endif
