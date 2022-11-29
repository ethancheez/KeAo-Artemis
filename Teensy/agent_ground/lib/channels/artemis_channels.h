#ifndef _ARTEMIS_CHANNELS_H
#define _ARTEMIS_CHANNELS_H

#include <artemis_defs.h>
#include <errno.h>
#include <TeensyThreads.h>
#include <rfm23.h>
#include <rfm98.h>
#include <astrodev.h>
#include <support/packetcomm.h>

namespace Artemis
{
    namespace Teensy
    {
        namespace Channels
        {
            void rfm23_channel();
            void rfm98_channel();
            void astrodev_channel();
        }
    }
}

#endif // _ARTEMIS_CHANNELS_H
