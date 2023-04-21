#include "channels/artemis_channels.h"
#include <i2c_driver_wire.h>
#include <pdu.h>

void sendToPi();

namespace
{
    using namespace Artemis;
    PacketComm packet;
}

void Artemis::Teensy::Channels::rpi_channel()
{
    Serial2.begin(9600);

    // Wait for Pi to boot
    while (!digitalRead(UART6_RX))
        continue;

    Serial.println("RPI Thread Starting...");

    while (true)
    {
        if (PullQueue(packet, rpi_queue, rpi_queue_mtx))
        {
            Serial.println((uint16_t)packet.header.type);
            switch (packet.header.type)
            {
            case PacketComm::TypeId::CommandEpsSwitchName:
                if ((Artemis::Teensy::PDU::PDU_SW)packet.data[0] == Artemis::Teensy::PDU::PDU_SW::RPI && packet.data[1] == 0)
                {
                    packet.header.type = PacketComm::TypeId::CommandObcHalt;
                    sendToPi();

                    threads.delay(20000); // Wait 20s just to be safe
                    digitalWrite(RPI_ENABLE, LOW);

                    // Empty RPI Queue
                    while (!rpi_queue.empty())
                        rpi_queue.pop_front();

                    Serial.println("Kill RPI Thread");
                    kill_thread(Artemis::Teensy::Channels::Channel_ID::RPI_CHANNEL);
                    return;
                }
                break;
            default:
                sendToPi();
                break;
            }
        }
        threads.delay(10);
    }
}

void sendToPi()
{
    if (!digitalRead(UART6_RX))
    {
        Serial.println("Unable to connect to Pi");
        return;
    }

    packet.SLIPPacketize();
    Serial.println("Forwarding to pi... ");
    for (size_t i = 0; i < packet.packetized.size(); i++)
    {
        Serial.print((unsigned)packet.packetized[i]);
        Serial.print(" ");
        Serial2.write(packet.packetized[i]);
    }
    Serial.println();
}