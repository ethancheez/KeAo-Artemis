#ifndef _CMD_QUEUE_H
#define _CMD_QUEUE_H

#include <queue>
#include <TeensyThreads.h>
#include <support/packetcomm.h>

// Mutex
extern Threads::Mutex main_queue_mtx;
extern Threads::Mutex astrodev_queue_mtx;
extern Threads::Mutex rfm23_queue_mtx;
extern Threads::Mutex rfm98_queue_mtx;
extern Threads::Mutex pdu_queue_mtx;
extern Threads::Mutex temperature_queue_mtx;
extern Threads::Mutex magnetometer_queue_mtx;
extern Threads::Mutex current_queue_mtx;
extern Threads::Mutex accelerometer_gyroscope_queue_mtx;

// Command Queues
extern std::queue<Cosmos::Support::PacketComm> main_queue;
extern std::queue<Cosmos::Support::PacketComm> astrodev_queue;
extern std::queue<Cosmos::Support::PacketComm> rfm23_queue;
extern std::queue<Cosmos::Support::PacketComm> rfm98_queue;
extern std::queue<Cosmos::Support::PacketComm> pdu_queue;
extern std::queue<Cosmos::Support::PacketComm> temperature_queue;
extern std::queue<Cosmos::Support::PacketComm> magnetometer_queue;
extern std::queue<Cosmos::Support::PacketComm> current_queue;
extern std::queue<Cosmos::Support::PacketComm> accelerometer_gyroscope_queue;

#endif // _CMD_QUEUE_H
