#include "agent/agentclass.h"
#include "device/rfm69/rfm69.h"
#include "device/bus/SPIDevice.h"

#define NODE 1
#define OTHERNODE 2
#define NETWORK 1

// Use /dev/spidev1.1
#define SPI_BUS     1
#define SPI_CHANNEL 1

#define MAXBUFFERSIZE 2560

using namespace std;
using namespace cubesat;

string agent_name = "radio2";

string node_name;

uint8_t spi_bus = SPI_BUS;
uint8_t spi_channel = SPI_CHANNEL;
SPIDevice::SPIMODE spi_mode = SPIDevice::MODE3; // default SPI mode

uint8_t freqBand = RF69_915MHZ; // default Frequency of Radio Module
uint8_t networkID = NETWORK; // default network ID

Agent *agent;
RFM69HCW *rfm69;

int check_input(int argc, char *argv[]);
void print_usage();

int main(int argc, char *argv[])
{
    if(!check_input(argc, argv)) exit(EXIT_FAILURE);

    if (!(agent = new Agent(node_name, agent_name, 1, MAXBUFFERSIZE, (bool)false))) {
        printf("Failed to start agent\n");
        exit(EXIT_FAILURE);
    }

    rfm69 = new RFM69HCW(spi_bus, spi_channel, spi_mode, freqBand, networkID);
    rfm69->rfm69_setHighPower(1);   // Make sure to do this for RFM69HCW (anything with "H")

    while(agent->running()) {
        string msg = "TEST MESSAGE";
        cout << "TX >> " << msg << endl;

        if(rfm69->rfm69_sendWithRetry((uint8_t) OTHERNODE, msg, (uint8_t) msg.length(), (uint8_t) 2, (uint8_t) 10)) {
            cout << "ACK RECEIVED" << endl;
        } else {
            cout << "NO ACK RECEIVED" << endl;
            break;
        }
    }

    delete rfm69;
    agent->shutdown();
}

int check_input(int argc, char *argv[]) {
    switch (argc) {
    case 2:
        node_name = argv[1];
        break;
    case 5:
        spi_bus = stoi(argv[1]);
        spi_channel = stoi(argv[2]);
        switch(stoi(argv[3])) {
        case 0:
            spi_mode = SPIDevice::MODE0;
            break;
        case 1:
            spi_mode = SPIDevice::MODE1;
            break;
        case 2:
            spi_mode = SPIDevice::MODE2;
            break;
        case 3:
            spi_mode = SPIDevice::MODE3;
            break;
        }
        node_name = argv[4];
        break;
    default:
        print_usage();
        return 0;
    }

    return 1;
}

void print_usage() {
    printf("    Usage 1: agent_radio2 {nodename} \n\n");
    printf("    Usage 2: agent_radio2 {SPI Bus} {SPI Channel} {SPI Mode} {nodename} \n");
    printf("        Used for Debugging Purposes \n");
    printf("        - Possible SPI Busses:      0 1 \n");
    printf("        - Possible SPI Channels:    0 1 \n");
    printf("        - Possible SPI Modes:       0 1 2 3 \n");
}
