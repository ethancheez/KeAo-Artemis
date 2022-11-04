#include "artemis_channels.h"
#include <deque>
#include <Arduino.h>

#define ASTRODEV_BAUD 9600
using namespace Artemis::Teensy::Radio;

int32_t astrodev_init(HardwareSerial *new_serial, uint32_t speed);
bool astrodev_recv();

namespace
{
    Astrodev astrodev;
    Astrodev::frame frame;
    PacketComm packet;
}

void Artemis::Teensy::Channels::astrodev_channel()
{
    int32_t iretn = 0;
    iretn = astrodev_init(&Serial8, ASTRODEV_BAUD);
    if (iretn < 0)
    {
        Serial.println("Error initializing Astrodev radio. Exiting...");
        exit(-1);
    }
    Serial.println("[LI-3] Radio init successful");

    while (true)
    {
        if (PullQueue(&packet, astrodev_queue, astrodev_queue_mtx))
        {
        }
        if (astrodev_recv())
        {
            Serial.print("[LI-3] RECEIVED: [");
            for (size_t i = 0; i < packet.data.size(); i++)
            {
                Serial.print((char)packet.data[i]);
            }
            Serial.println("]");
        }
        delay(10);
    }
}

/* Helper Functions */
int32_t astrodev_init(HardwareSerial *new_serial, uint32_t speed)
{
    int32_t iretn = astrodev.Init(new_serial, speed);
    if (iretn < 0)
    {
        Serial.println("Error initializing Astrodev radio. Exiting...");
        exit(-1);
    }
    astrodev.tcv_configuration.interface_baud_rate = 0;
    astrodev.tcv_configuration.power_amp_level = 220;
    astrodev.tcv_configuration.rx_baud_rate = 1;
    astrodev.tcv_configuration.tx_baud_rate = 1;
    astrodev.tcv_configuration.ax25_preamble_length = 20;
    astrodev.tcv_configuration.ax25_postamble_length = 20;
    // astrodev.tcv_configuration.config1;
    // astrodev.tcv_configuration.config2;
    astrodev.tcv_configuration.rx_modulation = (uint8_t)Astrodev::Modulation::ASTRODEV_MODULATION_GFSK;
    astrodev.tcv_configuration.tx_modulation = (uint8_t)Astrodev::Modulation::ASTRODEV_MODULATION_GFSK;
    astrodev.tcv_configuration.tx_freq_high = 450000 / 65536;
    astrodev.tcv_configuration.tx_freq_low = 450000 % 65536;
    astrodev.tcv_configuration.rx_freq_high = 450000 / 65536;
    astrodev.tcv_configuration.rx_freq_low = 450000 % 65536;
    memcpy(astrodev.tcv_configuration.ax25_source, "SOURCE", 6);
    memcpy(astrodev.tcv_configuration.ax25_destination, "DESTIN", 6);

    int8_t retries = 5;

    while ((iretn = astrodev.SetTCVConfig()) < 0)
    {
        Serial.println("Resetting");
        astrodev.Reset();
        Serial.println("Failed to settcvconfig astrodev");
        if (--retries < 0)
        {
            return iretn;
        }
        threads.delay(5000);
    }
    Serial.println("SetTCVConfig successful");

    retries = 5;
    while ((iretn = astrodev.GetTCVConfig()) < 0)
    {
        Serial.println("Failed to gettcvconfig astrodev");
        if (--retries < 0)
        {
            return iretn;
        }
        threads.delay(5000);
    }
    Serial.println("GetTCVConfig successful");
    return 0;
}

bool astrodev_recv()
{
    packet.header.dest = 0;
    int32_t iretn = 0;
    iretn = astrodev.Receive(frame);
    if (iretn < 0)
    {
        return false;
    }

    packet.header.dest = NODES::TEENSY_NODE_ID;
    packet.header.orig = NODES::GROUND_NODE_ID;

    // Handle payload
    Astrodev::Command cmd = (Astrodev::Command)iretn;

    switch (cmd)
    {
    // 0 is Acknowledge-type, view comment on return types for Astrodev::Receive()
    case (Astrodev::Command)0:
        return false;
    case Astrodev::Command::GETTCVCONFIG:
    case Astrodev::Command::TELEMETRY:
        // Setup PacketComm packet stuff
        packet.header.type = PacketComm::TypeId::DataRadioResponse;
        packet.data.resize(frame.header.sizelo + 1);
        packet.data[0] = (uint8_t)cmd;
        memcpy(packet.data.data(), &frame.payload[0], frame.header.sizelo);
        break;
    case Astrodev::Command::RECEIVE:
        // Packets from the ground will be in PacketComm protocol
        Serial.println("in receive");
        packet.wrapped.resize(frame.header.sizelo);
        memcpy(packet.wrapped.data(), &frame.payload[0], frame.header.sizelo);
        iretn = packet.Unwrap();
        if (iretn < 0)
        {
            return false;
        }
        break;
    default:
        Serial.print("cmd ");
        Serial.print((uint16_t)cmd);
        Serial.println("not (yet) handled. Terminating...");
        return false;
    }
    return true;
}
