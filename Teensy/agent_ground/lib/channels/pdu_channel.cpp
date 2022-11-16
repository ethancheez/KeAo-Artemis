#include <artemis_channels.h>

#define VBATT_PAYLOAD 1

namespace
{
    Artemis::Teensy::PDU pdu(115200);
    PacketComm packet;
}

void Artemis::Teensy::Channels::pdu_channel()
{
    // Enable burn wire
    pdu.PDU_SWITCH(Artemis::Teensy::PDU::BURN, true);
    threads.delay(30000);
    pdu.PDU_SWITCH(Artemis::Teensy::PDU::BURN, false);

    // Enable Lithium-3 Payload on VBATT
    if (VBATT_PAYLOAD)
    {
        pdu.PDU_SWITCH(Artemis::Teensy::PDU::VBATT, true);
    }

    while (true)
    {
        if (PullQueue(packet, pdu_queue, pdu_queue_mtx))
        {
        }
    }
}
