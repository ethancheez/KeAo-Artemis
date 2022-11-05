#include "artemis_channels.h"

void receiveData(int byte_count);
void sendData();

namespace
{
    PacketComm packet;
    size_t send_index = 0;
    bool ready = false;
}

void Artemis::Teensy::Channels::rpi_channel()
{
    I2C_Wire1.begin(0x08);
    I2C_Wire1.onReceive(receiveData);
    I2C_Wire1.onRequest(sendData);

    packet.wrapped.resize(0);

    while (true)
    {
        if (!ready && PullQueue(&packet, rpi_queue, rpi_queue_mtx))
        {
            packet.Wrap();
            ready = true;
        }
        delay(100);
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
    if (!ready)
    {
        I2C_Wire1.write(255); // Send empty character when no packet available
        return;
    }

    Threads::Scope lock(i2c1_mtx);
    I2C_Wire1.write(packet.wrapped[send_index]);
    Serial.print(packet.wrapped[send_index], HEX);
    send_index++;
    if (send_index == packet.wrapped.size())
    {
        send_index = 0;
        packet.wrapped.resize(0);
        ready = false;
        Serial.println();
    }
}
