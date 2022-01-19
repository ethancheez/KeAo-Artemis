


# RFM69HCW on Raspberry Pi using wiringPi and COSMOS

The `rfm69` directory belongs in the `~/cosmos/source/projects/` directory.

## Installing wiringPi on Linux

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
	- Note: there will be an error `Oops: Unable to determine board revision`. You can ignore this since the linux machine isn't an actual Raspberry Pi. This is just so Qt Creator can recognize the wiringPi library while you edit code.
```
$ gpio -v
```

## Compiling on the Raspberry Pi

If you do not have the cross-compiler installed on your machine, you can transfer the COSMOS project into your Raspberry Pi and compile there. You can do this using FileZilla or whatever you are most comfortable with.

The directory to save the COSMOS project: `~/cosmos/source/projects/`

Navigate to `~/cosmos/source/projects/rfm69` and run the following commands to compile:
```
$ mkdir build
$ cd build
$ cmake ../source/
$ make -j2
$ sudo make -j2 install
$ mv agent_rfm69 ~/cosmos/bin
```

You should now be able to run the agent by running:
```
$ agent_rfm69
```