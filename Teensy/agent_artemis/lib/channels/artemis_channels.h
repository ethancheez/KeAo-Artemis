#ifndef _ARTEMIS_CHANNELS_H
#define _ARTEMIS_CHANNELS_H

#include <TeensyThreads.h>
#include <cmd_queue.h>
#include <rfm23.h>
#include <rfm98.h>
#include <Wire.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_INA219.h>
#include <pdu.h>
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
            void magnetometer_channel();
            void accelerometer_gyroscope_channel();
            void temperature_channel();
            void current_channel();
            void pdu_channel();
        }
    }
}

#endif // _ARTEMIS_CHANNELS_H
