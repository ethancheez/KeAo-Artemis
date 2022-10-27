#include <artemis_defs.h>

// Mutex for Command Queues
Threads::Mutex main_queue_mtx;
Threads::Mutex astrodev_queue_mtx;
Threads::Mutex rfm23_queue_mtx;
Threads::Mutex rfm98_queue_mtx;
Threads::Mutex pdu_queue_mtx;
Threads::Mutex temperature_queue_mtx;
Threads::Mutex magnetometer_queue_mtx;
Threads::Mutex current_queue_mtx;
Threads::Mutex accelerometer_gyroscope_queue_mtx;

// Command Queues
std::queue<Cosmos::Support::PacketComm> main_queue;
std::queue<Cosmos::Support::PacketComm> astrodev_queue;
std::queue<Cosmos::Support::PacketComm> rfm23_queue;
std::queue<Cosmos::Support::PacketComm> rfm98_queue;
std::queue<Cosmos::Support::PacketComm> pdu_queue;
std::queue<Cosmos::Support::PacketComm> temperature_queue;
std::queue<Cosmos::Support::PacketComm> magnetometer_queue;
std::queue<Cosmos::Support::PacketComm> current_queue;
std::queue<Cosmos::Support::PacketComm> accelerometer_gyroscope_queue;

// Other Mutex
Threads::Mutex spi_mtx;
Threads::Mutex spi1_mtx;
