#include "channels/artemis_channels.h"
#include "artemisbeacons.h"
#include <pdu.h>

namespace
{
    Artemis::Teensy::PDU pdu(&Serial1, 115200);
    PacketComm packet;
    Artemis::Teensy::PDU::pdu_packet pdu_packet;
    std::string response;
    unsigned long timeoutStart;
    elapsedMillis uptime;
}

void Artemis::Teensy::Channels::pdu_channel()
{
    while (!Serial1)
        ;
    threads.delay(5000); // Give the PDU some time to warm up...

    // Ensure PDU is communicating with Teensy
    pdu_packet.type = PDU::PDU_Type::CommandPing;
    while (1)
    {
        pdu.send(pdu_packet);
        pdu.recv(response);
        if (response[0] == (uint8_t)PDU::PDU_Type::DataPong + PDU_CMD_OFFSET)
        {
            Serial.println("PDU Connection Established");
            break;
        }
        threads.delay(100);
    }

    // Enable burn wire
    // TODO: ASYNC DELAY TO ALLOW WDT RESET
    pdu.set_switch(Artemis::Teensy::PDU::PDU_SW::BURN, true);
    threads.delay(10000);
    pdu.set_switch(Artemis::Teensy::PDU::PDU_SW::BURN, false);

    while (true)
    {
        if (PullQueue(packet, pdu_queue, pdu_queue_mtx))
        {
            switch (packet.header.type)
            {
            case PacketComm::TypeId::CommandEpsCommunicate:
            {
                pdu_packet.type = PDU::PDU_Type::CommandPing;

                timeoutStart = millis();
                while (1)
                {
                    pdu.send(pdu_packet);
                    pdu.recv(response);
                    if (response[0] == (uint8_t)PDU::PDU_Type::DataPong + PDU_CMD_OFFSET)
                    {
                        pdu_packet.type = PDU::PDU_Type::DataPong;
                        packet.header.nodedest = packet.header.nodeorig;
                        packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
                        PushQueue(packet, main_queue, main_queue_mtx);
                        break;
                    }

                    if (millis() - timeoutStart > 5000)
                    {
                        Serial.println("Unable to Ping PDU");
                        break;
                    }

                    threads.delay(100);
                }
                break;
            }
            case PacketComm::TypeId::CommandEpsSwitchName:
            {
                Artemis::Teensy::PDU::PDU_SW switchid = (Artemis::Teensy::PDU::PDU_SW)packet.data[0];
                pdu.set_switch(switchid, packet.data[1]);
                break;
            }
            case PacketComm::TypeId::CommandEpsSwitchStatus:
            {
                string response;
                Artemis::Teensy::PDU::PDU_SW switchid = (Artemis::Teensy::PDU::PDU_SW)packet.data[0];
                pdu.get_switch(switchid, response);

                if (switchid == Artemis::Teensy::PDU::PDU_SW::All)
                {
                    switchbeacon beacon;
                    beacon.deci = uptime;
                    for (size_t i = 1; i < response.length() - 2; i++)
                    {
                        beacon.sw[i - 1] = response[i] - PDU_CMD_OFFSET;
                    }
                    beacon.sw[12] = digitalRead(RPI_ENABLE);

                    packet.header.type = PacketComm::TypeId::DataObcBeacon;
                    packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
                    packet.header.nodedest = (uint8_t)NODES::GROUND_NODE_ID;
                    packet.data.resize(sizeof(beacon));
                    memcpy(packet.data.data(), &beacon, sizeof(beacon));
                    packet.header.chanin = 0;
                    packet.header.chanout = Artemis::Teensy::Channels::Channel_ID::RFM23_CHANNEL;
                    PushQueue(packet, rfm23_queue, rfm23_queue_mtx);
                }
                break;
            }
            default:
                break;
            }
        }

        // Update WDT
        // pdu.set_switch(Artemis::Teensy::PDU::PDU_SW::WDT, 1);
        // pdu.set_switch(Artemis::Teensy::PDU::PDU_SW::WDT, 0);

        threads.delay(10);
    }
}
