#include <stdio.h>
#include <iostream>
#include <string.h>
#include "device/rfm69/rfm69.h"
#include "support/timelib.h"
#include "agent/agentclass.h"
#include <wiringPi.h>

// Radio Settings
int NODE = 1;
int OTHERNODE = 2;
int NETWORK = 1;

#define ENCRYPT 1
#define ENCRYPTKEY "1234567890123456"

// RPi Pinouts
#define intPin  18
#define rstPin  22
#define spiBus  0

#define MAXBUFFERSIZE 2560

using namespace std;
using namespace cubesat;

string agent_name = "RF69_RADIO";
string node_name;
int useACK = 1;

// Radio Frequency
uint8_t FREQUENCY = RF69_915MHZ;
//uint8_t FREQUENCY = RF69_415MHZ;

Agent *agent;

// Agent Requests
int32_t request_setACK(string &request, string &response, Agent *);

int check_input(int argc, char *argv[]);
void print_usage();

int main(int argc, char *argv[])
{
    int32_t iretn;

    if(!check_input(argc, argv)) exit(EXIT_FAILURE);

    if (!(agent = new Agent(node_name, agent_name, 1, MAXBUFFERSIZE, (bool)false))) {
        printf("Failed to start agent\n");
        exit(EXIT_FAILURE);
    }

    // Add Agent Requests
    if ((iretn=agent->add_request("setACK", request_setACK)))
	exit (iretn);

    RFM69 *rfm69 = new RFM69();
    rfm69->initialize(FREQUENCY, NODE, NETWORK, intPin, rstPin, spiBus);
    rfm69->setHighPower(1);

    if(ENCRYPT)
	rfm69->encrypt((unsigned char*)ENCRYPTKEY);

    printf("Temperature: %d\n", rfm69->readTemperature(0));

    char string_to_send[RF69_MAX_DATA_LEN];

    while(agent->running()) {
	printf("TX >> ");
	//scanf("%s", string_to_send);
	fgets(string_to_send, RF69_MAX_DATA_LEN, stdin);

	if(useACK) {
		//if(rfm69->sendWithRetry(OTHERNODE, "RADIO 1", 8, 2, 6))
		if(rfm69->sendWithRetry(OTHERNODE, string_to_send, strlen(string_to_send), 2, 10))
			cout << "ACK Received" << endl;
		else
			cout << "No ACK Received" << endl;
	} else {
		rfm69->send(OTHERNODE, string_to_send, strlen(string_to_send), false);
	}
	
	int received = 1;
	rfm69->receiveBegin();
	int TIMEOUT = 3000;
	int start = millis();
	while(!(rfm69->receiveDone())) {
		if(millis()-start > (unsigned int)TIMEOUT) {
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

    delete rfm69;
    agent->shutdown();
}

int check_input(int argc, char *argv[]) {
    switch (argc) {
    case 4:
	useACK = stoi(argv[3]);
    case 3:
	if((string)argv[2] == "radio2") {
		NODE = 2;
		OTHERNODE = 1;
	}
	else {
		NODE = 1;
		OTHERNODE = 2;
	}
        node_name = argv[1];
	break;
    default:
        print_usage();
        return 0;
    }

    return 1;
}

void print_usage() {
    printf("    Usage 1: ./agent_radio2 {nodename} {radio#} \n");
    printf("		- nodename:	Name of the agent node\n");
    printf("		- radio#:	\"radio1\" or \"radio2\"\n");
    printf("		- Defaults:\n");
    printf("			useACK:    TRUE (1)\n");
    printf("\n");

    printf("    Usage 2: ./agent_radio2 {nodename} {radio#} {useACK} \n");
    printf("		- nodename:	Name of the agent node\n");
    printf("		- radio#:	\"radio1\" or \"radio2\"\n");
    printf("		- useACK:	TRUE (1) or FALSE (0)\n");
    printf("\n");
}

int32_t request_setACK(string &request, string &response, Agent *) {
    sscanf(request.c_str(), "setACK %d", &useACK);
    return 0;
}
