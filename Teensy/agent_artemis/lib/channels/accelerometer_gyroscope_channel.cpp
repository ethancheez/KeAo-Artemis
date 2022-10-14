#include "artemis_channels.h"
Adafruit_LSM6DSOX sox;

void Artemis::Teensy::Channels::accelerometer_gyroscope_channel()
{
    if (!sox.begin_I2C())
    sox.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
    sox.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
    sox.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
    sox.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);

    while (true)
    {
        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        sox.getEvent(&accel, &gyro, &temp);

        Serial.print("IMU Temperature ");
        Serial.print(temp.temperature);
        Serial.println(" deg C");

        /* Display the results (acceleration is measured in m/s^2) */
        Serial.print("Accel X: ");
        Serial.print(accel.acceleration.x);
        Serial.print(" \tY: ");
        Serial.print(accel.acceleration.y);
        Serial.print(" \tZ: ");
        Serial.print(accel.acceleration.z);
        Serial.println(" m/s^2 ");

        /* Display the results (rotation is measured in rad/s) */
        Serial.print("Gyro X: ");
        Serial.print(gyro.gyro.x);
        Serial.print(" \tY: ");
        Serial.print(gyro.gyro.y);
        Serial.print(" \tZ: ");
        Serial.print(gyro.gyro.z);
        Serial.println(" radians/s ");
        Serial.println();

        delay(5000);
    }
}