#include <cstdint>
#include "artemis_defs.h"

enum class TypeId : uint8_t
{
    None = 0,
    temperature = 1,
    current1 = 2,
    current2 = 3,
    imu = 4,
    mag = 5,
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