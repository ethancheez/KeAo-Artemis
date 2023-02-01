#include "tests/packet_tests.h"

void send_packet_tests(void)
{
    Cosmos::Support::PacketComm packet;

#ifdef TEST_I2C_PI
    packet.header.type = (PacketComm ::TypeId)200; // TODO: CommandTakePicture
    packet.header.nodeorig = NODES::TEENSY_NODE_ID;
    packet.header.nodedest = NODES::RPI_NODE_ID;
    packet.data.resize(0);
    PushQueue(packet, rpi_queue, rpi_queue_mtx);
#endif

#ifdef TEST_ASTRODEV
    packet.header.type = PacketComm::TypeId::DataObcPong;
    packet.header.nodeorig = NODES::TEENSY_NODE_ID;
    packet.header.nodedest = NODES::GROUND_NODE_ID;
    packet.header.chanorig = 0;
    packet.header.chandest = Artemis::Teensy::Channels::Channel_ID::ASTRODEV_CHANNEL;
    PushQueue(packet, main_queue, main_queue_mtx);
#endif

#ifdef TEST_PDU
    packet.header.type = PacketComm::TypeId::CommandEpsSwitchName;
    packet.header.nodeorig = NODES::GROUND_NODE_ID;
    packet.header.nodedest = NODES::TEENSY_NODE_ID;
    packet.data.resize(0);
    packet.data.push_back((uint8_t)Artemis::Teensy::PDU::PDU_SW::All);
    packet.data.push_back(1);
    PushQueue(packet, main_queue, main_queue_mtx);
#endif
}