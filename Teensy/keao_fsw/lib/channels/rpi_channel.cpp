#include "artemis_channels.h"

void receiveData(int byte_count);
void sendData();

namespace
{
    using namespace Artemis;
    PacketComm packet;
    bool ready = false;
}

void Artemis::Teensy::Channels::rpi_channel()
{
    I2C_Wire1.begin(0x08);
    I2C_Wire1.onReceive(receiveData);
    I2C_Wire1.onRequest(sendData);
    pinMode(PI_STATUS, INPUT);

    packet.wrapped.resize(0);

    while (true)
    {
        if (!ready && PullQueue(packet, rpi_queue, rpi_queue_mtx))
        {
            // Special case to turn Pi off and kill RPI Channel
            if (packet.header.type == PacketComm::TypeId::CommandEpsSwitchName)
            {
                if ((Artemis::Teensy::PDU::PDU_SW)packet.data[0] == Artemis::Teensy::PDU::PDU_SW::RPI && packet.data[1] == 0)
                {
                    // Wait for PI_STATUS to turn off
                    while (digitalRead(PI_STATUS))
                        ;
                    threads.delay(10000); // Wait 10s just to be safe
                    digitalWrite(RPI_ENABLE, LOW);

                    // Empty RPI Queue
                    while (!rpi_queue.empty())
                        rpi_queue.pop_front();

                    kill_thread(Artemis::Teensy::Channels::Channel_ID::RPI_CHANNEL);
                    return;
                }
            }

            packet.Wrap();
            ready = true;
        }
        threads.delay(100);
    }
}

void receiveData(int byte_count)
{
    if (byte_count == 0)
        return;

    Threads::Scope lock(i2c1_mtx);
    for (int i = 0; i < byte_count; i++)
    {
        byte data = I2C_Wire1.read();
        Serial.println((char)data);
    }
}

void sendData()
{
    Threads::Scope lock(i2c1_mtx);
    if (!ready)
        return;

    for (size_t i = 0; i < packet.wrapped.size(); i++)
    {
        I2C_Wire1.write(packet.wrapped[i]);
        Serial.print(packet.wrapped[i], HEX);
    }
    packet.wrapped.resize(0);
    ready = false;
    Serial.println();
}
