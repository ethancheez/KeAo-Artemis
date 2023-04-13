#ifndef _ARTEMIS_BEACONS_H
#define _ARTEMIS_BEACONS_H

#include <cstdint>
#include "config/artemis_defs.h"

enum class TypeId : uint8_t
{
    None,
    temperature,
    current1,
    current2,
    imu,
    mag,
    gps,
    sw,
};

struct __attribute__((packed)) temperaturebeacon
{
    TypeId type = TypeId::temperature;
    uint32_t deci = 0;
    float temperatureC[ARTEMIS_TEMP_SENSOR_COUNT];
};

struct __attribute__((packed)) currentbeacon1
{
    TypeId type = TypeId::current1;
    uint32_t deci = 0;
    float busvoltage[ARTEMIS_CURRENT_BEACON_1_COUNT];
    float current[ARTEMIS_CURRENT_BEACON_1_COUNT];
};

struct __attribute__((packed)) currentbeacon2
{
    TypeId type = TypeId::current2;
    uint32_t deci = 0;
    float busvoltage[ARTEMIS_CURRENT_SENSOR_COUNT - ARTEMIS_CURRENT_BEACON_1_COUNT];
    float current[ARTEMIS_CURRENT_SENSOR_COUNT - ARTEMIS_CURRENT_BEACON_1_COUNT];
};

struct __attribute__((packed)) magbeacon
{
    TypeId type = TypeId::mag;
    uint32_t deci = 0;
    float magx = 0, magy = 0, magz = 0;
};

struct __attribute__((packed)) imubeacon
{
    TypeId type = TypeId::imu;
    uint32_t deci = 0;
    float accelx = 0, accely = 0, accelz = 0;
    float gyrox = 0, gyroy = 0, gyroz = 0;
    float imutemp = 0;
};

struct __attribute__((packed)) gpsbeacon
{
    TypeId type = TypeId::gps;
    uint32_t deci = 0;
    float latitude = 0, longitude = 0, speed = 0, angle = 0, altitude = 0, satellites = 0;
};

struct __attribute__((packed)) switchbeacon
{
    TypeId type = TypeId::sw;
    uint32_t deci = 0;
    uint8_t sw[13];
};

#endif // _ARTEMIS_BEACONS_H
