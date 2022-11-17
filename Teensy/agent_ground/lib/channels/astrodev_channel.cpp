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
    Astrodev astrodev;
    Astrodev::frame frame;
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
            case PacketComm::TypeId::DataBeacon:
            case PacketComm::TypeId::DataPong:
            case PacketComm::TypeId::DataEpsResponse:
            case PacketComm::TypeId::DataRadioResponse:
            case PacketComm::TypeId::DataAdcsResponse:
            case PacketComm::TypeId::DataResponse:
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

bool astrodev_send()
{
    int32_t iretn = 0;
    int8_t retries = 0;
    while (true)
    {
        if (!astrodev.buffer_full.load())
        {
            // Attempt transmit if transfer bull is not full
            iretn = astrodev.Transmit(packet);
            Serial.print("Transmit iretn: ");
            Serial.println(iretn);
            if (iretn < 0)
            {
                return false;
            }
            // Transmit successful
            return true;
        }
        else
        {
            Serial.println("Buffer full, waiting a bit");
            // Retry 3 times
            if (++retries > 3)
            {
                return false;
            }
            // Wait until transfer buffer is not full
            threads.delay(100);
            // Let recv_loop handle getting the response back and clearing buffer_full flag
            iretn = astrodev.Ping(false);
            if (!astrodev.buffer_full.load())
            {
                Serial.println("buffer full flag cleared");
                threads.delay(10);
                // Return to start of loop to attempt transmit
                continue;
            }
        }
    }
}
