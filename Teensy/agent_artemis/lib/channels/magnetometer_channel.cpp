#include "artemis_channels.h"
Adafruit_LIS3MDL lis3mdl;

void Artemis::Teensy::Channels::magnetometer_channel()
{
    if (!lis3mdl.begin_I2C()) // hardware I2C mode, can pass in address & alt Wire

        lis3mdl.setIntThreshold(500);
    lis3mdl.configInterrupt(false, false, true, // enable z axis
                            true,               // polarity
                            false,              // don't latch
                            true);              // enabled!

    while (true)
    {
        sensors_event_t event;
        lis3mdl.getEvent(&event);
        /* Display the results (magnetic field is measured in uTesla) */
        Serial.print("Magn X: ");
        Serial.print(event.magnetic.x);
        Serial.print(" \tY: ");
        Serial.print(event.magnetic.y);
        Serial.print(" \tZ: ");
        Serial.print(event.magnetic.z);
        Serial.println(" uTesla ");
        Serial.println();

        delay(5000);
    }
}