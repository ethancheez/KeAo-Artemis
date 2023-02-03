#include "channels/artemis_channels.h"
#include <deque>
#include <Arduino.h>

/* TODO:    Test astrodev channel without astrodev threads
 *          See if we need individual threads for sending/receiving
 */

#define ASTRODEV_BAUD 9600
#define ASTRODEV_FREQ 435000
using namespace Artemis::Teensy::Radio;

int32_t astrodev_init(HardwareSerial *new_serial, uint32_t baud_rate);
int32_t astrodev_recv();
int32_t astrodev_send();

namespace
{
    using namespace Cosmos::Devices::Radios;
    using namespace Artemis;
    Astrodev astrodev;
    Astrodev::frame incoming_message;
    PacketComm packet;
    elapsedMillis telem_timer;
}

void Artemis::Teensy::Channels::astrodev_channel()
{
    int32_t iretn = 0;

    iretn = astrodev_init(&Serial2, ASTRODEV_BAUD);
    while (iretn < 0)
    {
        Serial.println("Error initializing Astrodev radio. Exiting...");
        threads.delay(1000);
        iretn = astrodev_init(&Serial2, ASTRODEV_BAUD);
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
                if (astrodev_send() < 0)
                {
                    Serial.println("[LI-3] Transmit Failed. Packet Lost");
                }
            default:
                break;
            }
        }

        // Grab Telemetry every 10 seconds, and check if we are still connected
        if (telem_timer > 10000)
        {
            astrodev.GetTelemetry(false);
            telem_timer = 0;
        }

        if (astrodev_recv())
        {
            Serial.print("[LI-3] RECEIVED: [");
            for (size_t i = 0; i < packet.data.size(); i++)
            {
                Serial.print((char)packet.data[i]);
            }
            Serial.println("]");
            // PushQueue(packet, main_queue, main_queue_mtx);
        }

        
        threads.delay(10);
    }
}

/* Helper Functions */
int32_t astrodev_init(HardwareSerial *new_serial, uint32_t baud_rate)
{
    int32_t iretn = 0;
    astrodev.Init(new_serial, baud_rate);
    if (iretn < 0)
    {
        Serial.println("Error initializing Astrodev radio. Exiting...");
        return -1;
    }

    iretn = astrodev.Connect();
    if (iretn < 0)
    {
        Serial.println("Error connecting to Astrodev radio.");
        return -1;
    }

    astrodev.tcv_configuration.interface_baud_rate = 0;
    astrodev.tcv_configuration.power_amp_level = 100;
    astrodev.tcv_configuration.rx_baud_rate = 1;
    astrodev.tcv_configuration.tx_baud_rate = 1;
    astrodev.tcv_configuration.ax25_preamble_length = 20;
    astrodev.tcv_configuration.ax25_postamble_length = 20;
    astrodev.tcv_configuration.rx_modulation = Cosmos::Devices::Radios::Astrodev::Modulation::ASTRODEV_MODULATION_GFSK;
    astrodev.tcv_configuration.tx_modulation = Cosmos::Devices::Radios::Astrodev::Modulation::ASTRODEV_MODULATION_GFSK;
    astrodev.tcv_configuration.tx_frequency = ASTRODEV_FREQ;
    astrodev.tcv_configuration.rx_frequency = ASTRODEV_FREQ;
    memcpy(astrodev.tcv_configuration.ax25_source, "SOURCE", 6);
    memcpy(astrodev.tcv_configuration.ax25_destination, "DESTIN", 6);
    memset(&astrodev.tcv_configuration.config1, 0, 2);
    memset(&astrodev.tcv_configuration.config2, 0, 2);

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
    Serial.print("Checking config settings... ");
    if (astrodev.tcv_configuration.interface_baud_rate != 0 ||
        astrodev.tcv_configuration.power_amp_level != 100 ||
        astrodev.tcv_configuration.rx_baud_rate != 1 ||
        astrodev.tcv_configuration.tx_baud_rate != 1 ||
        astrodev.tcv_configuration.ax25_preamble_length != 20 ||
        astrodev.tcv_configuration.ax25_postamble_length != 20 ||
        astrodev.tcv_configuration.rx_modulation != Cosmos::Devices::Radios::Astrodev::Modulation::ASTRODEV_MODULATION_GFSK ||
        astrodev.tcv_configuration.tx_modulation != Cosmos::Devices::Radios::Astrodev::Modulation::ASTRODEV_MODULATION_GFSK ||
        astrodev.tcv_configuration.tx_frequency != ASTRODEV_FREQ ||
        astrodev.tcv_configuration.rx_frequency != ASTRODEV_FREQ)
    {
        Serial.println("config mismatch detected!");
        return -1;
    }
    Serial.println("config check OK!");
    Serial.println("GetTCVConfig successful");

    return 0;
}

int32_t astrodev_recv()
{
    int32_t iretn = 0;
    // Receive
    iretn = astrodev.Receive(incoming_message);
    if (iretn >= 0)
    {
        // Handle rx radio messages
        switch (incoming_message.header.command)
        {
        // 0 is Acknowledge-type, view comment on return types for Astrodev::Receive()
        // Though this does mean that I can't distinguish between acks and noacks
        case (Astrodev::Command)0:
            return -1;
        case Astrodev::Command::NOOP:
        case Astrodev::Command::GETTCVCONFIG:
        case Astrodev::Command::TELEMETRY:
            // Setup PacketComm packet stuff
            packet.header.type = Cosmos::Support::PacketComm::TypeId::CommandRadioAstrodevCommunicate;
            packet.header.nodeorig = (uint8_t)NODES::TEENSY_NODE_ID;
            packet.header.nodedest = (uint8_t)NODES::TEENSY_NODE_ID;
            packet.data.resize(incoming_message.header.sizelo + 4);
            packet.data[0] = 0;
            packet.data[1] = 0x20;
            packet.data[2] = (uint8_t)incoming_message.header.command;
            packet.data[3] = incoming_message.header.sizelo;
            memcpy(packet.data.data() + 4, &incoming_message.payload[0], incoming_message.header.sizelo);
            Serial.println("got telem packet");
            break;
        case Astrodev::Command::RECEIVE:
            // Packets from the ground will be in PacketComm protocol
            Serial.print("in rx receive, bytes:");
            Serial.println(incoming_message.header.sizelo);
            packet.wrapped.resize(incoming_message.header.sizelo - 18);
            memcpy(packet.wrapped.data(), &incoming_message.payload[16], incoming_message.header.sizelo - 18);
            iretn = packet.Unwrap();
            if (iretn < 0)
            {
                Serial.println("Unwrap fail in RX radio recv");
                return -1;
            }
            PushQueue(packet, main_queue, main_queue_mtx);
            break;
        default:
            Serial.print("cmd ");
            Serial.print((uint16_t)incoming_message.header.command);
            Serial.println(" not (yet) handled. Terminating...");
            break;
        }
    }
    return 0;
}

int32_t astrodev_send()
{
    int32_t iretn = 0;
    uint8_t retries = 0;
    Serial.println(" | Transmit message...");
    // TODO: Attempt resend on NACK? Would be difficult to coordinate
    while(true)
    {
        if (!astrodev.buffer_full.load())
        {
            // Attempt transmit if transfer bull is not full
            iretn = astrodev.Transmit(packet);
            Serial.print("Transmit iretn: ");
            Serial.println(iretn);
            // Transmit successful
            break;
        }
        else
        {
            Serial.println("Buffer full, waiting a bit");
            // Retry 3 times
            if(++retries > 3)
            {
                return -1;
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
    };

    return 0;
}
