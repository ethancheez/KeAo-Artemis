#include <artemis_defs.h>

// Nodes
NodeData nodeData;

// Mutex for Command Queues
Threads::Mutex main_queue_mtx;
Threads::Mutex astrodev_queue_mtx;
Threads::Mutex rfm23_queue_mtx;
Threads::Mutex rfm98_queue_mtx;
Threads::Mutex pdu_queue_mtx;

// Command Queues
queue<PacketComm> main_queue;
queue<PacketComm> astrodev_queue;
queue<PacketComm> rfm23_queue;
queue<PacketComm> rfm98_queue;
queue<PacketComm> pdu_queue;

// Other Mutex
Threads::Mutex spi_mtx;
Threads::Mutex spi1_mtx;
