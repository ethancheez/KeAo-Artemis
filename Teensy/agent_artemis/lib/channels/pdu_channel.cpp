#include <artemis_channels.h>

namespace
{
    Artemis::Teensy::PDU pdu(115200);
    PacketComm packet;
}

void Artemis::Teensy::Channels::pdu_channel()
{
    // Enable burn wire
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
                PDU::pdu_packet pdu_packet;
                pdu_packet.type = PDU::PDU_Type::CommandPing;

                pdu.send(pdu_packet);

                unsigned long timeoutStart = millis();
                while (!pdu.recv())
                {
                    if (millis() - timeoutStart > 5000)
                    {
                        Serial.println("FAIL TO SEND CMD TO PDU");
                        break;
                    }
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
    }
}
