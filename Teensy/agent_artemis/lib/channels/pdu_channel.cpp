#include <artemis_channels.h>

namespace
{
    Artemis::Teensy::PDU pdu(115200);
    PacketComm packet;
}

void Artemis::Teensy::Channels::pdu_channel()
{
    // Enable burn wire
    pdu.PDU_SWITCH(Artemis::Teensy::PDU::PDU_CMD::BURN, true);
    threads.delay(10000);
    pdu.PDU_SWITCH(Artemis::Teensy::PDU::PDU_CMD::BURN, false);

    while (true)
    {
        if (PullQueue(packet, pdu_queue, pdu_queue_mtx))
        {
            Artemis::Teensy::PDU::PDU_CMD switchid = (Artemis::Teensy::PDU::PDU_CMD)packet.data[0];
            switch (packet.header.type)
            {
            case PacketComm::TypeId::CommandEpsSwitchName:
            {
                switch (switchid)
                {
                case Artemis::Teensy::PDU::PDU_CMD::SW_3V3_1:
                case Artemis::Teensy::PDU::PDU_CMD::SW_3V3_2:
                case Artemis::Teensy::PDU::PDU_CMD::SW_5V_1:
                case Artemis::Teensy::PDU::PDU_CMD::SW_5V_2:
                case Artemis::Teensy::PDU::PDU_CMD::SW_5V_3:
                case Artemis::Teensy::PDU::PDU_CMD::SW_5V_4:
                case Artemis::Teensy::PDU::PDU_CMD::SW_12V:
                case Artemis::Teensy::PDU::PDU_CMD::VBATT:
                case Artemis::Teensy::PDU::PDU_CMD::WDT:
                case Artemis::Teensy::PDU::PDU_CMD::HBRIDGE1:
                case Artemis::Teensy::PDU::PDU_CMD::HBRIDGE2:
                case Artemis::Teensy::PDU::PDU_CMD::BURN:
                    pdu.PDU_SWITCH(switchid, packet.data[1]);
                    break;
                default:
                    break;
                }
            }
            default:
                break;
            }
        }
    }
}
