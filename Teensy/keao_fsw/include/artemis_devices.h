#ifndef _ARTEMIS_DEVICES_H
#define _ARTEMIS_DEVICES_H

#include <support/configCosmosKernel.h>
#include <artemis_defs.h>
#include "artemisbeacons.h"
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DSOX.h>
#include <Adafruit_INA219.h>

namespace Artemis
{
    class Devices
    {
    public:
        Adafruit_LIS3MDL magnetometer;
        Adafruit_LSM6DSOX imu;

        // Current Sensors
        std::map<std::string, Adafruit_INA219 *> current_sensors = {
            {"solar_panel_1", new Adafruit_INA219(0x40)},
            {"solar_panel_2", new Adafruit_INA219(0x41)},
            {"solar_panel_3", new Adafruit_INA219(0x42)},
            {"solar_panel_4", new Adafruit_INA219(0x43)},
            {"battery_board", new Adafruit_INA219(0x44)},
        };

        // Temperature Sensors
        std::map<std::string, int> temp_sensors = {
            {"obc", A0},
            {"pdu", A1},
            {"battery_board", A6},
            {"solar_panel_1", A7},
            {"solar_panel_2", A8},
            {"solar_panel_3", A9},
            {"solar_panel_4", A17},
        };

        int32_t setup_magnetometer(void);
        int32_t setup_imu(void);
        int32_t setup_current(void);
        int32_t setup_temperature(void);
        int32_t read_temperature(uint32_t uptime);
        int32_t read_current(uint32_t uptime);
        int32_t read_imu(uint32_t uptime);
        int32_t read_mag(uint32_t uptime);
    };
}

#endif //_ARTEMIS_DEVICES_H
