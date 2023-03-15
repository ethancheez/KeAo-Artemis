#include "tests/packet_tests.h"
#include <pdu.h>

using namespace Artemis;

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

#ifdef TEST_PDU
    packet.header.type = PacketComm::TypeId::CommandEpsSwitchName;
    packet.header.nodeorig = (uint8_t)NODES::GROUND_NODE_ID;
    packet.header.nodedest = (uint8_t)NODES::TEENSY_NODE_ID;
    packet.data.resize(0);
    packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::All);
    packet.data.push_back(1);
    PushQueue(packet, main_queue, main_queue_mtx);
    packet.data.resize(0);
    packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::All);
    packet.data.push_back(0);
    PushQueue(packet, main_queue, main_queue_mtx);
#endif
}