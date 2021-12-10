//#include "agent/agentclass.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "rfm69.h"

// CHANGE THESE FOR EVERY NODE

#define NODE 1
#define OTHERNODE 2
#define NETWORK 1     // THIS STAYS THE SAME

#define ENCRYPT     1                       // (1) Enable Encryption, (0) Disable Encryption
#define ENCRYPTKEY  "1234567890123456"      // must be 16 bytes

// RPi Pinouts
#define intPin  18
#define rstPin  22
#define spiBus  0

#define MAXBUFFERSIZE 2560

using namespace std;
using namespace cubesat;

string agent_name = "radio2";
string node_name;
int useACK = 1;

// Radio Frequency
uint8_t FREQUENCY = RF69_915MHZ;
//uint8_t FREQUENCY = RF69_415MHZ;

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
	delay(1000);
	printf("TX >> RADIO 1\n");
	//scanf("%s", string_to_send);
	//fgets(string_to_send, RF69_MAX_DATA_LEN, stdin);

	if(useACK) {
		if(rfm69->sendWithRetry(OTHERNODE, "RADIO 1", 8, 2, 6))
		//if(rfm69->sendWithRetry(OTHERNODE, string_to_send, strlen(string_to_send), 2, 10))
			cout << "ACK Received" << endl;
		else
			cout << "No ACK Received" << endl;
	} else {
		//rfm69->send(OTHERNODE, string_to_send, strlen(string_to_send), false);
		rfm69->send(OTHERNODE, "RADIO 1", 8, false);
	}
	
	int received = 1;
	rfm69->receiveBegin();
	int TIMEOUT = 4000;
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
    case 3:
	useACK = stoi(argv[2]);
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
    printf("    Usage 1: ./ping_pong1 {nodename} \n");
    printf("		- nodename:	Name of the agent node\n");
    printf("		- Defaults:\n");
    printf("			useACK:    TRUE (1)\n");
    printf("\n");

    printf("    Usage 2: ./ping_pong1 {nodename} {useACK} \n");
    printf("		- nodename:	Name of the agent node\n");
    printf("		- useACK:	TRUE (1) or FALSE (0)\n");
    printf("\n");
}
