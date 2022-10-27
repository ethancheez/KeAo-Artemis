#ifndef _ARTEMIS_DEFS_H
#define _ARTEMIS_DEFS_H

#include <queue>
#include <TeensyThreads.h>
#include <support/packetcomm.h>

// Sensor Defs
#define ARTEMIS_CURRENT_SENSOR_COUNT 5
#define ARTEMIS_TEMP_SENSOR_COUNT 7
#define AREF_VOLTAGE 3.3

// Structs
struct thread_struct
{
  int thread_id;
  const char *thread_name;
};

// Enums
enum TEENSY_PINS
{
    UART4_RXD,
    UART4_TXD,
    T_GPIO2,
    T_GPIO3,
    T_GPIO4,
    T_GPIO5,
    T_GPIO6,
    RADIO_RESET,
    NIRQ,
    T_CS1,
    T_CS,
    SPI0_MOSI,
    SPI0_MISO,
    SPI0_SCLK,
    AIN0,
    AIN1,
    SCL1_I2C,
    SDA1_I2C,
    I2C2_SDA,
    I2C2_SCL,
    AIN3,
    AIN4,
    AIN5,
    AIN6,
    I2C1_SCL,
    I2C1_SDA,
    SPI1_D1,
    SPI1_SCLK,
    UART5_TXD,
    UART5_RXD,
    RX_ON,
    TX_ON,
    PI_STATUS,
    GPS_RSTN,
    UART2_RXD,
    UART2_TXD,
    RPI_ENABLE,
    SDN,
    SPI1_CS1,
    SPI1_D0,
    A16_UNUSED,
    AIN2
};

// Mutex for Command Queues
extern Threads::Mutex main_queue_mtx;
extern Threads::Mutex astrodev_queue_mtx;
extern Threads::Mutex rfm23_queue_mtx;
extern Threads::Mutex rfm98_queue_mtx;
extern Threads::Mutex pdu_queue_mtx;

// Command Queues
extern std::queue<Cosmos::Support::PacketComm> main_queue;
extern std::queue<Cosmos::Support::PacketComm> astrodev_queue;
extern std::queue<Cosmos::Support::PacketComm> rfm23_queue;
extern std::queue<Cosmos::Support::PacketComm> rfm98_queue;
extern std::queue<Cosmos::Support::PacketComm> pdu_queue;

// Other Mutex
extern Threads::Mutex spi_mtx;
extern Threads::Mutex spi1_mtx;

#endif  // _ARTEMIS_DEFS_H
