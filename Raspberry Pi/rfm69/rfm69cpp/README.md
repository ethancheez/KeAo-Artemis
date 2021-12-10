

# KeAo - RFM69HCW on Raspberry Pi using the wiringPi Library

This is a documentation of getting the RFM69HCW connected to the Raspberry Pi using the wiringPi library. Note that using the wiringPi library does not use any COSMOS dependencies. It is purely compiled on the Raspberry Pi using g++.

## Overview
This directory contains four different codes corresponding to radio1, radio2, ping_pong1, and ping_pong2. 

**Radio1**: Created only to send packets entered by the user to radio2.
**Radio2**: Created only to receive packets from radio1.
**Ping_Pong**: There are two files that make two radios send and receive packets to each other.

## RFM69HCW Pinouts

| RFM69HCW | RPi |
| ---| ---|
| 3V3 | 17 |
| GND | 20 |
| MISO | 21 |
| MOSI | 19 |
| SCK | 23 |
| NSS | 24 (GPIO 8) (CE0) |
| RESET | 22 (GPIO 25) |
| DIO0 | 18 (GPIO 24) |

## wiringPi
To run the cpp files in this directory, the wiringPi library is required.

1. Check if you have wiringPi installed. Run `gpio -v`. 
	- If the command isn't recognized, then wiringPi isn't installed. Go to step 3.
	- If the command is recognized, you have wiringPi. If you wish to update it, go to step 2.
2. If wiringPi is installed, and you want to update it, run the following commands:
```
$ sudo apt-get purge wiringpi
$ hash -r
```
3. Install git if it isn't already installed. 
```
$ sudo apt-get install git-core
```
4. Clone wiringPi GitHub repo:
```
$ git clone https://github.com/WiringPi/WiringPi.git
```
5. Run the following commands to install the library:
```
$ cd WiringPi
$ git pull origin
$ ./build
```
6. Check to make sure wiringPi is installed:
```
$ gpio -v
$ gpio readall
```

## Compiling

There should me a makefile in the directory. You can compile everything by entering the following:
```
$ make
```
If you wish to compile separate programs, you can run any of these separately:
```
$ make radio1
$ make radio2
$ make ping_pong1
$ make ping_pong2
```

## Execution

### Radio1
Enter the following to display the usage of radio1:
```
$ ./radio1
```
Essentially, you can pass in two arguments. The first is the name of the node (this is mainly for COSMOS stuff). The second is to tell the program to use acknowledgements (ACKs) or not. An example execution:
```
$ ./radio1 node_1 1
```

### Radio2
Enter the following to display the usage of radio2:
```
$ ./radio2
```
This is simpler, you just need to pass in the name of the node (again, mainly for COSMOS stuff, kind of irrelevant for this set of programs). An example execution:
```
$ ./radio2 node_2
```

### Ping Pong
To just have the two Raspberry Pis automatically send/receive packets from each other, you can run:
```
$ ./ping_pong1 {node_name} {useACK}
```
and
```
$ ./ping_pong2 {node_name} {useACK}
```