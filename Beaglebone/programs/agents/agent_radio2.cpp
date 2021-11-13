#include "agent/agentclass.h"
#include "device/rfm69/rfm69.h"
#include "device/bus/SPIDevice.h"

// CHANGE THESE FOR EVERY NODE

#define NODE 1
#define OTHERNODE 2
#define NETWORK 1     // THIS STAYS THE SAME

// Using /dev/spidev1.1
#define SPI_BUS     1
#define SPI_CHANNEL 1

#define ENCRYPT     1                       // (1) Enable Encryption, (0) Disable Encryption
#define ENCRYPTKEY  "1234567890123456"      // must be 16 bytes

#define useACK      0

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

    rfm69 = new RFM69HCW(spi_bus, spi_channel, spi_mode, freqBand, (uint8_t) NODE, networkID);
    rfm69->rfm69_setHighPower(1);   // Make sure to do this for RFM69HCW (anything with "H")

    if(ENCRYPT)
        rfm69->rfm69_encrypt(ENCRYPTKEY);

    while(agent->running()) {
        string msg = "TEST";
        cout << "TX >> " << msg << endl;

        if(useACK) {
            if(rfm69->rfm69_sendWithRetry((uint8_t) OTHERNODE, msg, (uint8_t) msg.length(), (uint8_t) 2, (uint8_t) 10)) {
                cout << "ACK RECEIVED" << endl;
            } else {
                cout << "NO ACK RECEIVED... Shutting Down" << endl;
                break;
            }
        } else {
            rfm69->rfm69_send((uint8_t) OTHERNODE, msg, (uint8_t) msg.length(), false);
            cout << "Packet Sent" << endl;
        }


        /* Receiving */
        rfm69->rfm69_receiveBegin();
        uint8_t TIMEOUT = 5;
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        while(!(rfm69->rfm69_receiveDone())) {
            //cout << "receiving..." << endl;
            if(elapsed_seconds.count() > TIMEOUT) {
                cout << "Nothing Received" << endl;
                break;
            }
            end = std::chrono::steady_clock::now();
            elapsed_seconds = end - start;
        }

        if(elapsed_seconds.count() < TIMEOUT) {
            printf("Received from node %d: ", rfm69->senderID);
            for(uint8_t i = 0; i < rfm69->dataLen; i++) {
                printf("%c", rfm69->DATA[i]);
            }
            printf(" [RSSI: %d]\n", rfm69->rssi);

            if(rfm69->rfm69_ACKRequested()) {
                rfm69->rfm69_sendACK("ACK", 3);
                printf("ACK SENT\n");
            }
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
