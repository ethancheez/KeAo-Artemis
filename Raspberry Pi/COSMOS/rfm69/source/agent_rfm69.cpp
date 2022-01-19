#include <stdio.h>
#include <iostream>
#include <string.h>
#include "rfm69.h"
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
int useACK = 0;

// Radio Frequency
uint8_t FREQUENCY = RF69_915MHZ;
//uint8_t FREQUENCY = RF69_415MHZ;

Agent *agent;
RFM69 *rfm69;

// Agent Settings
int SENDING_ON = 1;
int RECEIVING_ON = 1;

// Agent Requests
int32_t request_toggleACK(string &request, string &response, Agent *);
int32_t request_setNODE(string &request, string &response, Agent *);
int32_t request_setOTHERNODE(string &request, string &response, Agent *);
int32_t request_setNETWORK(string &request, string &response, Agent *);
int32_t request_toggleSending(string &request, string &response, Agent *);
int32_t request_toggleReceiving(string &request, string &response, Agent *);
int32_t request_transferFile(string &request, string &response, Agent *);

int check_input(int argc, char *argv[]);
void print_usage();
void print_requests();

int main(int argc, char *argv[])
{
    int32_t iretn;
    bool transfering_file = false;
    bool open_file = false;
    FILE *fp;

    if(!check_input(argc, argv)) exit(EXIT_FAILURE);

    if (!(agent = new Agent(node_name, agent_name, 1, MAXBUFFERSIZE, (bool)false))) {
        printf("Failed to start agent\n");
        exit(EXIT_FAILURE);
    }

    // Add Agent Requests
    if ((iretn=agent->add_request("toggleACK", request_toggleACK))) exit (iretn);
    if ((iretn=agent->add_request("setNODE", request_setNODE))) exit (iretn);
    if ((iretn=agent->add_request("setOTHERNODE", request_setOTHERNODE))) exit (iretn);
    if ((iretn=agent->add_request("setNETWORK", request_setNETWORK))) exit (iretn);
    if ((iretn=agent->add_request("toggleSending", request_toggleSending))) exit (iretn);
    if ((iretn=agent->add_request("toggleReceiving", request_toggleReceiving))) exit (iretn);
    if ((iretn=agent->add_request("transferFile", request_transferFile))) exit (iretn);

    rfm69 = new RFM69();
    rfm69->initialize(FREQUENCY, NODE, NETWORK, intPin, rstPin, spiBus);
    rfm69->setHighPower(1);

    if(ENCRYPT)
        rfm69->encrypt((unsigned char*)ENCRYPTKEY);

    printf("Temperature: %d\n", rfm69->readTemperature(0));

    char string_to_send[RF69_MAX_DATA_LEN];

    while(agent->running()) {
        if(SENDING_ON) {
            printf("TX >> ");
            //scanf("%s", string_to_send);
            fgets(string_to_send, RF69_MAX_DATA_LEN, stdin);

            if(useACK) {
                //if(rfm69->sendWithRetry(OTHERNODE, "RADIO 1", 8, 2, 6))
                if(rfm69->sendWithRetry(OTHERNODE, string_to_send, strlen(string_to_send), 5, 50))
                    cout << "ACK Received" << endl;
                else
                    cout << "No ACK Received" << endl;
            } else {
                rfm69->send(OTHERNODE, string_to_send, strlen(string_to_send), false);
            }
        }

        if(RECEIVING_ON) {
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
                char RECEIVED_DATA[RF69_MAX_DATA_LEN];
                memset(RECEIVED_DATA, 0, sizeof RECEIVED_DATA);

                for(uint8_t i = 1; i < rfm69->DATALEN; i++) {
                    RECEIVED_DATA[i-1] = rfm69->DATA[i];
                }

                //cout << "received: " << RECEIVED_DATA << endl;
                if(strstr(RECEIVED_DATA, "FILE TRANSFER START") && !transfering_file) {
                    transfering_file = true;
                    open_file = true;
                    cout << "Starting File Download . . ." << endl;
                }
                else if(strstr(RECEIVED_DATA, "FILE TRANSFER END") && transfering_file) {
                    transfering_file = false;
                    fclose(fp);
                    cout << "Download Complete!" << endl;
                }
                else if(open_file) {
                    char file_name[RF69_MAX_DATA_LEN];
                    memset(file_name, 0, sizeof file_name);
                    strcpy(file_name, RECEIVED_DATA);
                    fp = fopen(file_name, "wb+");
                    open_file = false;
                    cout << "Created File: " << file_name << endl;
                    cout << "Downloading Data . . ." << endl;
                }
                else if(transfering_file) {
                    for(uint8_t i = 1; i < rfm69->DATALEN; i++) {
                        fputc(rfm69->DATA[i], fp);
                    }
                    //cout << "datalen: " << (int)(rfm69->DATALEN) << endl;
                }
                else {
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
        }

    }

    delete rfm69;
    agent->shutdown();
}

int check_input(int argc, char *argv[]) {
    switch (argc) {
        case 4:
            SENDING_ON = stoi(argv[3]);
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
        case 2:
            if((string)argv[1] == "requests")
                print_requests();
            else
                print_usage();
            return 0;
        default:
            print_usage();
            return 0;
    }

    return 1;
}

void print_usage() {
    printf("Incomplete Command:\n");
    printf("    Usage 1: ./agent_rfm69 requests \n");
    printf("		- List all agent requests for agent_rfm69\n");
    printf("\n");

    printf("    Usage 2: ./agent_rfm69 {nodename} {radio#} \n");
    printf("		- nodename:	Name of the agent node\n");
    printf("		- radio#:	\"radio1\" or \"radio2\"\n");
    printf("\n");

    printf("    Usage 3: ./agent_rfm69 {nodename} {radio#} {Sending} \n");
    printf("		- nodename:	Name of the agent node\n");
    printf("		- radio#:	\"radio1\" or \"radio2\"\n");
    printf("		- Sending:	ON (1) or OFF (0)\n");
    printf("\n");
}

void print_requests() {
    printf("setNODE (int):          Set node of current radio\n");
    printf("setOTHERNODE (int):     Set node of other radio\n");
    printf("setNETWORK (int):       Set network of radio. This should be the same as other radios communicating with each other\n");
    printf("toggleACK:              Toggle acknowledgements when sending/receiving data\n");
    printf("toggleSending:          Toggle sending function for current radio\n");
    printf("toggleReceiving:        Toggle receiving function for current radio\n");
    printf("transferFile (string):  Transfer file from current node to other node\n");
}

int32_t request_toggleACK(string &request, string &response, Agent *) {
    useACK = !useACK;
    if(useACK)
        cout << "ACK Enabled" << endl;
    else
        cout << "ACK Disabled" << endl;
    return 0;
}

int32_t request_setNODE(string &request, string &response, Agent *) {
    sscanf(request.c_str(), "setNODE %d", &NODE);
    return 0;
}

int32_t request_setOTHERNODE(string &request, string &response, Agent *) {
    sscanf(request.c_str(), "setOTHERNODE %d", &OTHERNODE);
    return 0;
}

int32_t request_setNETWORK(string &request, string &response, Agent *) {
    sscanf(request.c_str(), "setNETWORK %d", &NETWORK);
    return 0;
}

int32_t request_toggleSending(string &request, string &response, Agent *) {
    SENDING_ON = !SENDING_ON;
    if(SENDING_ON)
        cout << "Sending turned ON" << endl;
    else
        cout << "Sending turned OFF" << endl;
    return 0;
}

int32_t request_toggleReceiving(string &request, string &response, Agent *) {
    RECEIVING_ON = !RECEIVING_ON;
    if(RECEIVING_ON)
        cout << "Receiving turned ON" << endl;
    else
        cout << "Receiving turned OFF" << endl;
    return 0;
}

int32_t request_transferFile(string &request, string &response, Agent *) {
    FILE *fp;
    char file_name[RF69_MAX_DATA_LEN];
    char string_to_send[RF69_MAX_DATA_LEN];

    memset(file_name, 0, sizeof file_name);
    memset(string_to_send, 0, sizeof string_to_send);
    sscanf(request.c_str(), "transferFile %s", file_name);

    if( (fp = fopen(file_name, "rb")) == NULL ) {
        cout << "Error opening file:" << file_name << endl;
        return 1;
    }

    rfm69->send(OTHERNODE, "FILE TRANSFER START", 20, false);
    rfm69->send(OTHERNODE, file_name, strlen(file_name)+1, false);

    //cout << "file len: " << strlen(file_name) << endl;

    while(fgets(string_to_send, RF69_MAX_DATA_LEN-1, fp)) {
        //cout << "fgets: " << string_to_send << "      strlen: " << strlen(string_to_send) << endl;
        if(useACK) {
            if(rfm69->sendWithRetry(OTHERNODE, string_to_send, strlen(string_to_send)+1, 5, 50))
                cout << "ACK Received" << endl;
            else
                cout << "No ACK Received" << endl;
        } else {
            rfm69->send(OTHERNODE, string_to_send, strlen(string_to_send)+1, false);
        }
        memset(string_to_send, 0, sizeof string_to_send);
    }

    cout << "File Transfer Complete" << endl;
    rfm69->send(OTHERNODE, "FILE TRANSFER END", 18, false);

    fclose(fp);
    return 0;
}
