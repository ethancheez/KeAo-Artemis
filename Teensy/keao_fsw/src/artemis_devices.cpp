#include "artemis_devices.h"
#include "channels/artemis_channels.h"

namespace Artemis
{
    int32_t Devices::setup_magnetometer(void)
    {
        if (!magnetometer->begin_I2C())
        {
            return -1;
        }

        magnetometer->setPerformanceMode(LIS3MDL_LOWPOWERMODE);
        magnetometer->setDataRate(LIS3MDL_DATARATE_0_625_HZ);
        magnetometer->setRange(LIS3MDL_RANGE_16_GAUSS);
        magnetometer->setOperationMode(LIS3MDL_CONTINUOUSMODE);

        return 0;
    }

    int32_t Devices::read_mag(uint32_t uptime)
    {
        PacketComm packet;
        magbeacon beacon;
        beacon.deci = uptime;

        sensors_event_t event;
        magnetometer->getEvent(&event);
        beacon.magx = (event.magnetic.x);
        beacon.magy = (event.magnetic.y);
        beacon.magz = (event.magnetic.z);

        packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
        packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
        packet.header.type = PacketComm::TypeId::DataObcBeacon;
        packet.data.resize(sizeof(beacon));
        memcpy(packet.data.data(), &beacon, sizeof(beacon));
        packet.header.chanorig = 0;
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);

        return 0;
    }

    int32_t Devices::setup_imu(void)
    {
        if (!imu->begin_I2C())
        {
            return -1;
        }
        imu->setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
        imu->setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
        imu->setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
        imu->setGyroDataRate(LSM6DS_RATE_6_66K_HZ);

        return 0;
    }

    int32_t Devices::read_imu(uint32_t uptime)
    {
        PacketComm packet;
        imubeacon beacon;
        beacon.deci = uptime;

        sensors_event_t accel;
        sensors_event_t gyro;
        sensors_event_t temp;
        imu->getEvent(&accel, &gyro, &temp);

        beacon.accelx = (accel.acceleration.x);
        beacon.accely = (accel.acceleration.y);
        beacon.accelz = (accel.acceleration.z);
        beacon.gyrox = (gyro.gyro.x);
        beacon.gyroy = (gyro.gyro.y);
        beacon.gyroz = (gyro.gyro.z);
        beacon.imutemp = (temp.temperature);

        packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
        packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
        packet.header.type = PacketComm::TypeId::DataObcBeacon;
        packet.data.resize(sizeof(beacon));
        memcpy(packet.data.data(), &beacon, sizeof(beacon));
        packet.header.chanorig = 0;
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);

        return 0;
    }

    int32_t Devices::setup_current(void) // go through library and see what we need to configure and callibrate
    {
        for (auto &it : current_sensors)
        {
            if (it.second->begin(&Wire2))
            {
                return -1;
            }
        }

        return 0;
    }

    int32_t Devices::read_current(uint32_t uptime)
    {
        PacketComm packet;

        currentbeacon1 beacon1;
        currentbeacon2 beacon2;
        packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
        packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
        packet.header.type = PacketComm::TypeId::DataObcBeacon;

        for (auto &it : current_sensors)
        {
            const int i = std::distance(current_sensors.begin(), current_sensors.find(it.first));
            if (i < ARTEMIS_CURRENT_BEACON_1_COUNT)
            {
                beacon1.busvoltage[i] = (current_sensors[it.first]->getBusVoltage_V());
                beacon1.current[i] = (current_sensors[it.first]->getCurrent_mA());
            }
            else
            {
                beacon2.busvoltage[i - ARTEMIS_CURRENT_BEACON_1_COUNT] = (current_sensors[it.first]->getBusVoltage_V());
                beacon2.current[i - ARTEMIS_CURRENT_BEACON_1_COUNT] = (current_sensors[it.first]->getCurrent_mA());
            }
        }

        beacon1.deci = uptime;
        packet.data.resize(sizeof(beacon1));
        memcpy(packet.data.data(), &beacon1, sizeof(beacon1));
        packet.header.chanorig = 0;
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);

        beacon2.deci = uptime;
        packet.data.resize(sizeof(beacon2));
        memcpy(packet.data.data(), &beacon2, sizeof(beacon2));
        packet.header.chanorig = 0;
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);

        return 0;
    }

    int32_t Devices::setup_temperature(void)
    {
        for (auto &it : temp_sensors)
        {
            pinMode(it.second, INPUT);
        }

        return 0;
    }

    int32_t Devices::read_temperature(uint32_t uptime)
    {
        PacketComm packet;
        temperaturebeacon beacon;
        beacon.deci = uptime;
        for (auto &it : temp_sensors)
        {
            const int reading = analogRead(it.second);
            float voltage = reading * AREF_VOLTAGE;
            voltage /= 1024.0;
            const float temperatureF = (voltage * 1000) - 58;
            beacon.temperatureC[std::distance(temp_sensors.begin(), temp_sensors.find(it.first))] = (temperatureF - 32) / 1.8;
        }
        packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
        packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
        packet.header.type = PacketComm::TypeId::DataObcBeacon;
        packet.data.resize(sizeof(beacon));
        memcpy(packet.data.data(), &beacon, sizeof(beacon));
        packet.header.chanorig = 0;
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);

        return 0;
    }

    int32_t Devices::setup_gps(void)
    {
        gps->begin(9600);
        threads.delay(100);
        gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        threads.delay(100);
        gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
        threads.delay(100);

        return 0;
    }

    int32_t Devices::update_gps(void)
    {
        if (gps->available())
        {
            while (gps->read())
                ;
        }

        if (gps->newNMEAreceived()) // Check to see if a new NMEA line has been received
        {
            if (gps->parse(gps->lastNMEA())) // A successful message was parsed
            {
                // Serial.print(gps->lastNMEA());
            }
        }

        return 0;
    }

    int32_t Devices::read_gps(uint32_t uptime)
    {
        // TODO: Store in beacons / internal structures
        Serial.print("\nTime: ");

        if (gps->hour < 10)
        {
            Serial.print('0');
        }

        Serial.print(gps->hour, DEC);
        Serial.print(':');

        if (gps->minute < 10)
        {
            Serial.print('0');
        }

        Serial.print(gps->minute, DEC);
        Serial.print(':');

        if (gps->seconds < 10)
        {
            Serial.print('0');
        }

        Serial.print(gps->seconds, DEC);
        Serial.print('.');

        if (gps->milliseconds < 10)
        {
            Serial.print("00");
        }
        else if (gps->milliseconds > 9 && gps->milliseconds < 100)
        {
            Serial.print("0");
        }

        Serial.println(gps->milliseconds);
        Serial.print("Date: ");
        Serial.print(gps->day, DEC);
        Serial.print('/');
        Serial.print(gps->month, DEC);
        Serial.print("/20");
        Serial.println(gps->year, DEC);
        Serial.print("Fix: ");
        Serial.print((int)gps->fix);
        Serial.print(" quality: ");
        Serial.println((int)gps->fixquality);

        if (gps->fix)
        {
            Serial.print("Location (DDDMM.MMMM): ");
            Serial.print(gps->latitude, 4);
            Serial.print(gps->lat);
            Serial.print(", ");
            Serial.print(gps->longitude, 4);
            Serial.println(gps->lon);
            Serial.print("Location (Decimal Degree): ");
            Serial.print(gps->latitudeDegrees);
            Serial.print(", ");
            Serial.println(gps->longitudeDegrees);
            Serial.print("Speed (knots): ");
            Serial.println(gps->speed);
            Serial.print("Angle: ");
            Serial.println(gps->angle);
            Serial.print("Altitude: ");
            Serial.println(gps->altitude);
            Serial.print("Satellites: ");
            Serial.println((int)gps->satellites);
        }

        return 0;
    }
}
