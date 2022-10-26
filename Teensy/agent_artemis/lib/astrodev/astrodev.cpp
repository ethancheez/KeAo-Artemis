#include "astrodev.h"

namespace Artemis
{
    namespace Teensy
    {
        namespace Radio
        {
            Astrodev::Astrodev()
            {
            }

            int32_t Astrodev::Init(uint32_t baud_rate)
            {
                int32_t iretn;

                Serial8.begin(baud_rate);

                if ((iretn=Ping()) < 0)
                {
                    Reset();
                    delay(20000);
                    if ((iretn=Ping()) < 0)
                    {
                        return iretn;
                    }
                }

                return 0;
            }

            int32_t Astrodev::Transmit(frame message)
            {
                Threads::Scope scope(qmutex_out);

                if (message.header.size > MTU)
                {
                    return -1;
                }

                message.header.sync0 = SYNC0;
                message.header.sync1 = SYNC1;
                message.header.type = COMMAND;
                message.preamble[4] = 0;
                message.header.cs = CalcCS(&message.preamble[2], 4);
                for (uint16_t i = 0; i < 8; i++)
                {
                    Serial8.print(message.preamble[i]);
                }

                union
                {
                    uint16_t cs;
                    uint8_t csb[2];
                };
                cs = CalcCS(&message.preamble[2], 6 + message.header.size);
                message.payload[message.header.size] = csb[0];
                message.payload[message.header.size + 1] = csb[1];
                for (uint16_t i = 0; i < message.header.size + 2; i++)
                {
                    Serial8.print(message.payload[i]);
                }

                return message.header.size + 10;
            }

            int32_t Astrodev::Ping()
            {
                int32_t iretn;
                frame message;

                message.header.command = (uint8_t)Command::NOOP;
                message.header.size = 0;
                iretn = Transmit(message);
                return iretn;
            }

            int32_t Astrodev::Reset()
            {
                int32_t iretn;
                frame message;

                message.header.command = (uint8_t)Command::RESET;
                message.header.size = 0;
                iretn = Transmit(message);
                return iretn;
            }

            int32_t Astrodev::SendData(std::vector<uint8_t> data)
            {
                int32_t iretn;
                frame message;

                if (data.size() > MTU)
                {
                    return -1;
                }
                message.header.command = (uint8_t)Command::RESET;
                message.header.size = data.size();
                memcpy(message.payload, &data[0], data.size());
                iretn = Transmit(message);
                return iretn;
            }

            int32_t Astrodev::GetTCVConfig()
            {
                int32_t iretn;
                frame message;

                message.header.command = (uint8_t)Command::GETTCVCONFIG;
                message.header.size = 0;
                iretn = Transmit(message);
                return iretn;
            }

            int32_t Astrodev::SetTCVConfig(tcv_config config)
            {
                int32_t iretn;
                frame message;

                message.header.command = (uint8_t)Command::SETTCVCONFIG;
                message.header.size = sizeof(tcv_config);
                message.tcv = config;
                iretn = Transmit(message);
                return iretn;
            }

            int32_t Astrodev::GetTelemetry()
            {
                int32_t iretn;
                frame message;

                message.header.command = (uint8_t)Command::TELEMETRY;
                message.header.size = 0;
                iretn = Transmit(message);
                return iretn;
            }

            int32_t Astrodev::SetRFConfig(rf_config config)
            {
                int32_t iretn;
                frame message;

                message.header.command = (uint8_t)Command::RFCONFIG;
                message.header.size = sizeof(rf_config);
                message.rf = config;
                iretn = Transmit(message);
                return iretn;
            }

            uint16_t Astrodev::CalcCS(uint8_t *data, uint16_t size)
            {
                uint8_t ck_a = 0, ck_b = 0;
                uint16_t cs;

                for (uint16_t i = 0; i < size; ++i)
                {
                    ck_a += data[i];
                    ck_b += ck_a;
                }
                cs = ck_a | (ck_b << 8L);

                return (cs);
            }
        }
    }
}
