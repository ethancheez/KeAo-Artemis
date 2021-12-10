//#include "agent/agentclass.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "rfm69.h"

// CHANGE THESE FOR EVERY NODE

#define NODE 2
#define OTHERNODE 1
#define NETWORK 1     // THIS STAYS THE SAME

#define ENCRYPT     1                       // (1) Enable Encryption, (0) Disable Encryption
#define ENCRYPTKEY  "1234567890123456"      // must be 16 bytes

// RPi Pinouts
#define intPin  18 //36
#define rstPin  22
#define spiBus  0

//#define useACK      1

#define MAXBUFFERSIZE 2560

using namespace std;
using namespace cubesat;

string agent_name = "radio2";

string node_name;

uint8_t FREQUENCY = RF69_915MHZ; // default Frequency of Radio Module

//Agent *agent;

int check_input(int argc, char *argv[]);
void print_usage();

int main(int argc, char *argv[])
{
    if(!check_input(argc, argv)) exit(EXIT_FAILURE);

    /*if (!(agent = new Agent(node_name, agent_name, 1, MAXBUFFERSIZE, (bool)false))) {
        printf("Failed to start agent\n");
        exit(EXIT_FAILURE);
    }*/

    RFM69 *rfm69 = new RFM69();
    rfm69->initialize(FREQUENCY, NODE, NETWORK, intPin, rstPin, spiBus);
    printf("Temperature: %d\n", rfm69->readTemperature(0));

    char string_to_send[RF69_MAX_DATA_LEN];

    while(1) {
	/*
	printf("TX >> RADIO 1\n");
	//scanf("%s", string_to_send);
	//fgets(string_to_send, RF69_MAX_DATA_LEN, stdin);

	if(rfm69->sendWithRetry(OTHERNODE, "RADIO 2", 8, 2, 10))
	//if(rfm69->sendWithRetry(OTHERNODE, string_to_send, strlen(string_to_send), 2, 10))
		cout << "ACK Received" << endl;
	else
		cout << "No ACK Received" << endl;
	*/
	int received = 1;
	rfm69->receiveBegin();
	int TIMEOUT = 5000;
	int start = millis();
	while(!(rfm69->receiveDone())) {
		if(millis()-start > TIMEOUT) {
			cout << "RX << Nothing Received" << endl;
			received = 0;
			break;
		}
	}

	if(received) {
		cout << "Received from node " << rfm69->SENDERID << ": ";
		for(uint8_t i = 1; i < rfm69->DATALEN; i++) {
			cout << rfm69->DATA[i];
		}
		cout << " [RSSI: " << rfm69->RSSI << "]" << endl;

		if(rfm69->ACKRequested()) {
			rfm69->sendACK("ACK", 4);
			cout << "ACK SENT" << endl;
		}
	}
    }
    /*while(agent->running()) {
        break;
    }*/

    delete rfm69;
    //agent->shutdown();
}

int check_input(int argc, char *argv[]) {
    switch (argc) {
    case 2:
        node_name = argv[1];
        break;
    default:
        print_usage();
        return 0;
    }

    return 1;
}

void print_usage() {
    printf("    Usage 1: ./radio2 {nodename} \n\n");
}
