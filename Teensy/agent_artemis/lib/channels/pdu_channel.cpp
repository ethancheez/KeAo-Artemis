#include <artemis_channels.h>

namespace
{
    Artemis::Teensy::PDU pdu(115200);
    PacketComm packet;
    Artemis::Teensy::PDU::pdu_packet pdu_packet;
    std::string response;
    unsigned long timeoutStart;
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
                        packet.header.dest = packet.header.orig;
                        packet.header.orig = NODES::TEENSY_NODE_ID;
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
                Artemis::Teensy::PDU::PDU_SW switchid = (Artemis::Teensy::PDU::PDU_SW)packet.data[0];
                pdu.get_switch(switchid);
                break;
            }
            default:
                break;
            }
        }

        // Update WDT
        // pdu.set_switch(Artemis::Teensy::PDU::PDU_SW::WDT, 1);
        // pdu.set_switch(Artemis::Teensy::PDU::PDU_SW::WDT, 0);
    }
}
