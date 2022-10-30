#include <Arduino.h>
#include <vector>
#include <artemis_channels.h>
#include <support/datalib.h>
#include <support/configCosmos.h>

namespace
{
  NodeData::NODE_ID_TYPE ground_node_id;
  NodeData::NODE_ID_TYPE teensy_node_id;
  NodeData::NODE_ID_TYPE rpi_node_id;
  NodeData::NODE_ID_TYPE pleiades_node_id;

  PacketComm packet;
}

/* Helper Function Defs */
bool setup_magnetometer(void);
bool setup_imu(void);
void setup_current(void);
void setup_temperature(void);
void read_temperature(void);
void read_current(void);
void read_imu(void);

Adafruit_LIS3MDL magnetometer;
Adafruit_LSM6DSOX imu;
Adafruit_INA219 current_1(0x40); // Solar 1
Adafruit_INA219 current_2(0x41); // Solar 2
Adafruit_INA219 current_3(0x42); // Solar 3
Adafruit_INA219 current_4(0x43); // Solar 4
Adafruit_INA219 current_5(0x44); // Battery

// Current Sensors
const char *current_sen_names[ARTEMIS_CURRENT_SENSOR_COUNT] = {"solar_panel_1", "solar_panel_2", "solar_panel_3", "solar_panel_4", "battery_board"};
float busvoltage[ARTEMIS_CURRENT_SENSOR_COUNT] = {0}, current[ARTEMIS_CURRENT_SENSOR_COUNT] = {0}, power[ARTEMIS_CURRENT_SENSOR_COUNT] = {0};
Adafruit_INA219 *p[ARTEMIS_CURRENT_SENSOR_COUNT] = {&current_1, &current_2, &current_3, &current_4, &current_5};

// Temperature Sensors
const int temps[ARTEMIS_TEMP_SENSOR_COUNT] = {A0, A1, A6, A7, A8, A9, A17};
const char *temp_sen_names[ARTEMIS_TEMP_SENSOR_COUNT] = {"solar_panel_1", "solar_panel_2", "solar_panel_3", "solar_panel_4", "battery_board"};
float voltage[ARTEMIS_TEMP_SENSOR_COUNT] = {0}, temperatureC[ARTEMIS_TEMP_SENSOR_COUNT] = {0};

// IMU
float magx = {0}, magy = {0}, magz = {0};
float accelx = {0}, accely = {0}, accelz = {0};
float gyrox = {0}, gyroy = {0}, gyroz = {0};
float imutemp = {0};

void setup()
{
  Serial.begin(115200);
  delay(3000);

  // Initialize Node IDs
  ground_node_id = nodeData.lookup_node_id("ground");
  teensy_node_id = nodeData.lookup_node_id("teensy");
  rpi_node_id = nodeData.lookup_node_id("rpi");
  pleiades_node_id = nodeData.lookup_node_id("pleiades");

  // setup_magnetometer();
  // setup_imu();
  // setup_current();

  // Threads
  thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm23_channel), "rfm23 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::rfm98_channel), "rfm98 thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::pdu_channel), "pdu thread"});
  // thread_list.push_back({threads.addThread(Artemis::Teensy::Channels::astrodev_channel), "astrodev thread"});

  packet.header.orig = teensy_node_id;
  packet.header.dest = ground_node_id;
  packet.header.radio = ARTEMIS_RADIOS::RFM23;
  packet.header.type = PacketComm::TypeId::CommandTestRadio;

  packet.PushQueue(main_queue, main_queue_mtx);
}

void loop()
{
  if (packet.PullQueue(main_queue, main_queue_mtx))
  {
    if (packet.header.dest == ground_node_id)
    {
      switch (packet.header.radio)
      {
      case ARTEMIS_RADIOS::RFM23:
        packet.PushQueue(rfm23_queue, rfm23_queue_mtx);
        break;
      case ARTEMIS_RADIOS::ASTRODEV:
        packet.PushQueue(astrodev_queue, astrodev_queue_mtx);
        break;
      }
    }
    else if (packet.header.dest == rpi_node_id)
    {
    }
    else if (packet.header.dest == pleiades_node_id)
    {
      packet.PushQueue(rfm98_queue, rfm98_queue_mtx);
    }
    else if (packet.header.dest == teensy_node_id)
    {
      switch (packet.header.type)
      {
      case PacketComm::TypeId::CommandEpsCommunicate:
      case PacketComm::TypeId::CommandEpsMinimumPower:
      case PacketComm::TypeId::CommandEpsReset:
      case PacketComm::TypeId::CommandEpsSetTime:
      case PacketComm::TypeId::CommandEpsState:
      case PacketComm::TypeId::CommandEpsSwitchName:
      case PacketComm::TypeId::CommandEpsSwitchNames:
      case PacketComm::TypeId::CommandEpsSwitchNumber:
      case PacketComm::TypeId::CommandEpsSwitchStatus:
      case PacketComm::TypeId::CommandEpsWatchdog:
        packet.PushQueue(pdu_queue, pdu_queue_mtx);
        break;
      default:
        break;
      }
    }
  }
}

/* Helper Functions */
bool setup_magnetometer(void)
{
  if (!magnetometer.begin_I2C())
  {
    return false;
  }

  magnetometer.setPerformanceMode(LIS3MDL_LOWPOWERMODE);
  magnetometer.setDataRate(LIS3MDL_DATARATE_0_625_HZ);
  magnetometer.setRange(LIS3MDL_RANGE_16_GAUSS);
  magnetometer.setOperationMode(LIS3MDL_CONTINUOUSMODE);

  return true;
}

bool setup_imu(void)
{
  if (!imu.begin_I2C())
  {
    return false;
  }
  imu.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
  imu.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
  imu.setAccelDataRate(LSM6DS_RATE_6_66K_HZ);
  imu.setGyroDataRate(LSM6DS_RATE_6_66K_HZ);

  return true;
}

void setup_current(void) // go through library and see what we need to configure and callibrate
{
  current_1.begin(&Wire2);
  current_2.begin(&Wire2);
  current_3.begin(&Wire2);
  current_4.begin(&Wire2);
  current_5.begin(&Wire2);

  return;
}

void setup_temperature(void)
{
  for (const int pin : temps)
  {
    pinMode(pin, INPUT);
  }

  return;
}
void read_tempertaure(void) // future make this its own library
{
  for (int i = 0; i < ARTEMIS_TEMP_SENSOR_COUNT; i++)
  {
    const int reading = analogRead(temps[i]);
    voltage[i] = reading * AREF_VOLTAGE;
    voltage[i] /= 1024.0;
    const float temperatureF = (voltage[i] * 1000) - 58;
    temperatureC[i] = (temperatureF - 32) / 1.8;
  }
}

void read_current(void)
{
  for (int i = 0; i < ARTEMIS_CURRENT_SENSOR_COUNT; i++)
  {
    busvoltage[i] = (p[i]->getBusVoltage_V());
    current[i] = (p[i]->getCurrent_mA());
    power[i] = (p[i]->getPower_mW());
  }
}

void read_imu(void)
{
  sensors_event_t event;
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp);
  magnetometer.getEvent(&event);
  magx = (event.magnetic.x);
  magy = (event.magnetic.y);
  magz = (event.magnetic.z);
  accelx = (accel.acceleration.x);
  accely = (accel.acceleration.y);
  accelz = (accel.acceleration.z);
  gyrox = (gyro.gyro.x);
  gyroy = (gyro.gyro.y);
  gyroz = (gyro.gyro.z);
  imutemp = (temp.temperature);
}
