#ifndef _ARTEMIS_CHANNELS_H
#define _ARTEMIS_CHANNELS_H

#include <artemis_defs.h>
#include <errno.h>
#include <TeensyThreads.h>
#include <rfm23.h>
#include <rfm98.h>
#include <astrodev.h>
#include <Wire.h>
#include <i2c_driver_wire.h>
#include <pdu.h>
#include <support/packetcomm.h>

namespace Artemis
{
    namespace Teensy
    {
        namespace Channels
        {
            // Channel IDs
            enum Channel_ID : uint8_t
            {
                RFM23_CHANNEL = 1,
                RFM98_CHANNEL,
                ASTRODEV_CHANNEL,
                PDU_CHANNEL,
                RPI_CHANNEL,
            };

            void rfm23_channel();
            void rfm98_channel();
            void astrodev_channel();
            void pdu_channel();
            void rpi_channel();
        }
    }
}

#endif // _ARTEMIS_CHANNELS_H
