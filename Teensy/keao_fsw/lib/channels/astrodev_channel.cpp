#include "artemis_channels.h"
#include <deque>
#include <Arduino.h>

/* TODO:    Test astrodev channel without astrodev threads
 *          See if we need individual threads for sending/receiving
 */

#define ASTRODEV_BAUD 9600
using namespace Artemis::Teensy::Radio;

int32_t astrodev_init(HardwareSerial *new_serial, uint32_t speed);
bool astrodev_recv();
bool astrodev_send();

namespace
{
    PacketComm packet;
}

void Artemis::Teensy::Channels::astrodev_channel()
{
    int32_t iretn = 0;
    iretn = astrodev_init(&Serial8, ASTRODEV_BAUD); // Set to Serial5 for HyTi setup
    if (iretn < 0)
    {
        Serial.println("Error initializing Astrodev radio. Exiting...");
        exit(-1);
    }
    Serial.println("[LI-3] Radio init successful");

    packet.wrapped.resize(0);

    while (true)
    {
        if (PullQueue(packet, astrodev_queue, astrodev_queue_mtx))
        {
            switch (packet.header.type)
            {
            case PacketComm::TypeId::DataObcBeacon:
            case PacketComm::TypeId::DataObcPong:
            case PacketComm::TypeId::DataEpsResponse:
            case PacketComm::TypeId::DataRadioResponse:
            case PacketComm::TypeId::DataAdcsResponse:
            case PacketComm::TypeId::DataObcResponse:
                // No need to wrap packet, handled in astrodev library
                if (!astrodev_send())
                {
                    Serial.println("[LI-3] Transmit Failed. Packet Lost");
                }
            default:
                break;
            }
        }
        if (astrodev_recv())
        {
            Serial.print("[LI-3] RECEIVED: [");
            for (size_t i = 0; i < packet.data.size(); i++)
            {
                Serial.print((char)packet.data[i]);
            }
            Serial.println("]");
            PushQueue(packet, main_queue, main_queue_mtx);
        }
        threads.delay(10);
    }
}

/* Helper Functions */
int32_t astrodev_init(HardwareSerial *new_serial, uint32_t speed)
{
}

bool astrodev_recv()
{

}

bool astrodev_send()
{
}
