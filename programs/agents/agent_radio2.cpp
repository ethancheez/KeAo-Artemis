/********************************************************************
* Copyright (C) 2015 by Interstel Technologies, Inc.
*   and Hawaii Space Flight Laboratory.
*
* This file is part of the COSMOS/core that is the central
* module for COSMOS. For more information on COSMOS go to
* <http://cosmos-project.com>
*
* The COSMOS/core software is licenced under the
* GNU Lesser General Public License (LGPL) version 3 licence.
*
* You should have received a copy of the
* GNU Lesser General Public License
* If not, go to <http://www.gnu.org/licenses/>
*
* COSMOS/core is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* COSMOS/core is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* Refer to the "licences" folder for further information on the
* condititons and terms to use this software.
********************************************************************/

#include "agent/agentclass.h"
#include "device/rfm69/rfm69.h"
#include "device/bus/SPIDevice.h"

#define MAXBUFFERSIZE 2560

using namespace std;
using namespace cubesat;

string node_name = "obcradio"; // default node name
string agent_name = "radio2";

uint8_t spi_bus = 1; // default SPI config to /dev/spidev1.0
uint8_t dev_addr = 0;
SPIDevice::SPIMODE spi_mode = SPIDevice::MODE3; // default SPI mode
uint8_t networkID = 1;  // default network ID

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

    rfm69 = new RFM69HCW(spi_bus, dev_addr, spi_mode);
    rfm69->rfm69_setHighPower(1);   // Make sure to do this for RFM69HCW (anything with "H")

    while(agent->running()) {
        //read_register = rfm69->rfm69_read(dev_addr);
        break;
    }

    delete rfm69;
    agent->shutdown();
}

int check_input(int argc, char *argv[]) {
    switch (argc) {
    case 3:
        node_name = argv[1];
        networkID = stoi(argv[2]);
        break;
    case 6:
        spi_bus = stoi(argv[1]);
        dev_addr = stoi(argv[2]);
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
        networkID = stoi(argv[5]);
        break;
    default:
        print_usage();
        return 0;
    }

    return 1;
}

void print_usage() {
    printf("    Usage 1: agent_radio2 {nodename} {Network ID} \n");
    printf("        - This will default the serial interface to spidev1.0 and MODE3 \n\n");
    printf("    Usage 2: agent_radio2 {SPI Bus} {SPI Channel} {SPI Mode} {nodename} {Network ID} \n");
    printf("        - Possible SPI Busses:      0 1 \n");
    printf("        - Possible SPI Channels:    0 1 \n");
    printf("        - Possible SPI Modes:       0 1 2 3 \n");
}
