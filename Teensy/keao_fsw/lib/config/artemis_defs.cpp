#include <artemis_defs.h>
#include <artemis_channels.h>

vector<struct thread_struct> thread_list;

std::map<string, NODES> NodeType = {
    {"ground", NODES::GROUND_NODE_ID},
    {"artemis_teensy", NODES::TEENSY_NODE_ID},
    {"artemis_rpi", NODES::RPI_NODE_ID},
    {"pleiades", NODES::PLEIADES_NODE_ID},
};

// Mutex for Command Queues
Threads::Mutex main_queue_mtx;
Threads::Mutex astrodev_queue_mtx;
Threads::Mutex rfm23_queue_mtx;
Threads::Mutex rfm98_queue_mtx;
Threads::Mutex pdu_queue_mtx;
Threads::Mutex rpi_queue_mtx;

// Command Queues
std::deque<PacketComm> main_queue;
std::deque<PacketComm> astrodev_queue;
std::deque<PacketComm> rfm23_queue;
std::deque<PacketComm> rfm98_queue;
std::deque<PacketComm> pdu_queue;
std::deque<PacketComm> rpi_queue;

// Other Mutex
Threads::Mutex spi1_mtx;
Threads::Mutex i2c1_mtx;

// Utility Functions
int kill_thread(uint8_t channel_id)
{
    for (auto it = thread_list.begin(); it != thread_list.end(); it++)
    {
        if (it->channel_id == channel_id)
        {
            int ret = it->thread_id;
            threads.kill(it->thread_id);
            thread_list.erase(it);
            return ret;
        }
    }
    return -1;
}

int32_t PushQueue(PacketComm &packet, std::deque<PacketComm> &queue, Threads::Mutex &mtx)
{
    Threads::Scope lock(mtx);
    if (queue.size() > MAXQUEUESIZE)
    {
        queue.pop_front();
    }
    queue.push_back(packet);
    return 1;
}

int32_t PullQueue(PacketComm &packet, std::deque<PacketComm> &queue, Threads::Mutex &mtx)
{
    Threads::Scope lock(mtx);
    if (queue.size() > 0)
    {
        packet = queue.front();
        queue.pop_front();
        return 1;
    }
    else
    {
        return 0;
    }
}

int32_t PushCommQueue(PacketComm &packet)
{
    switch (Artemis::Teensy::Channels::comm_id)
    {
    case Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL:
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
        PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
        return 0;
        break;
    case Artemis::Teensy::Channels::Channel_ID::RFM98_CHANNEL:
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::RFM98_CHANNEL;
        PushQueue(packet, rfm98_queue, rfm98_queue_mtx);
        return 0;
        break;
    case Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL:
        packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
        PushQueue(packet, astrodev_queue, astrodev_queue_mtx);
        return 0;
        break;
    }
    return -1;
}
