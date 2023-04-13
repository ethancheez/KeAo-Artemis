#include "tests/packet_tests.h"
#include <pdu.h>

namespace
{
    using namespace Artemis;

    uint32_t packet_count = 0;
    elapsedMillis piShutdownTimer = 0;
    bool isoff = false;
}

void send_test_packets()
{
    Cosmos::Support::PacketComm packet;

#ifdef TEST_PI
    packet.header.type = PacketComm::TypeId::CommandEpsSwitchName;
    packet.header.nodeorig = (uint8_t)NODES::GROUND_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::TEENSY_NODE_ID;
    packet.data.resize(0);
    packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::RPI);
    packet.data.push_back(1);
    PushQueue(packet, main_queue, main_queue_mtx);
#endif

#ifdef TEST_I2C_PI
    packet.header.type = (PacketComm::TypeId)0x800; // TODO: CommandTakePicture
    packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::RPI_NODE_ID;
    packet.data.resize(0);
    PushQueue(packet, rpi_queue, rpi_queue_mtx);
#endif

#ifdef TEST_ASTRODEV
    packet.header.type = PacketComm::TypeId::DataObcPong;
    packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
    packet.header.chanin = 0;
    packet.header.chanout = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
    PushQueue(packet, main_queue, main_queue_mtx);
#endif

#ifdef TEST_RFM23
    packet.header.type = PacketComm::TypeId::DataObcResponse;
    packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
    packet.header.chanin = 0;
    packet.header.chanout = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;

    packet.data.resize(0);
    String data_str = String(packet_count);
    for (size_t i = 0; i < data_str.length(); i++)
    {
        packet.data.push_back(data_str[i]);
    }
    packet_count++;

    PushQueue(packet, main_queue, main_queue_mtx);
#endif

#ifdef TEST_PDU
    packet.header.type = PacketComm::TypeId::CommandEpsSwitchName;
    packet.header.nodeorig = (uint8_t)NODES::GROUND_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::TEENSY_NODE_ID;
    packet.data.resize(0);
    packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::All);
    packet.data.push_back(1);
    PushQueue(packet, main_queue, main_queue_mtx);
#endif

#ifdef TEST_CAMERACAPTURE
    packet.header.type = PacketComm::TypeId::CommandCameraCapture;
    packet.header.nodeorig = (uint8_t)NODES::GROUND_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::RPI_NODE_ID;
    packet.data.clear();
    PushQueue(packet, main_queue, main_queue_mtx);
#endif

#ifdef TEST_RPI_SHUTDOWN
    if (piShutdownTimer > 10000 && !isoff)
    {
        packet.header.type = PacketComm::TypeId::CommandEpsSwitchName;
        packet.header.nodeorig = (uint8_t)NODES::GROUND_NODE_ID;
        packet.header.nodedest = (uint8_t)NODES::RPI_NODE_ID;
        packet.data.clear();
        packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::RPI);
        packet.data.push_back(0);
        PushQueue(packet, main_queue, main_queue_mtx);
    }
#endif
}