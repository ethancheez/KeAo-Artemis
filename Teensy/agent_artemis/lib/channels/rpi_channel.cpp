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
        if (!ready && PullQueue(packet, rpi_queue, rpi_queue_mtx))
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
