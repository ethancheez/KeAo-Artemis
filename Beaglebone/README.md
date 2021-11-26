# KeAo - RFM69HCW on Beaglebone Black

This is a documentation of getting the RFM69HCW connected to the Beaglebone Black.

## Getting Started
Note: The pin configurations are set up based on the HSFL / Ke Ao OBC board.

1. Open up a command line and SSH into the BBB. You may be able to use `beaglebone.local` or the IP address the BBB is connected to in your network. i.e. `192.168.x.x`
```
$ ssh debian@beaglebone.local
```

### Setting up the Serial Interface
This will be a permanent solution to set up the serial interface upon boot of the BBB.

1. Open up the boot file using any text editor such as nano or vim. Note: you need admin permissions to edit this file.
```
$ sudo nano /boot/uEnv.txt
```
2. We need to enable the SPI1 pins on the BBB. 
In order to do so, add the following line under the `Additional custom capes` section.
```
uboot_overlay_addr4=/lib/firmware/BB-SPIDEV1-00A0.dtbo
```
- The following pins are included in this file:
	- P9_28	=>	SPI1_CS0
	- P9_29	=>	SPI1_D0
	- P9_30	=>	SPI1_D1
	- P9_31	=>	SPI1_SCLK
	- P9_42	=>	SPI1_CS1 (does not start on boot)

3. In order to use the SPI1 pins, the HDMI port MUST be disabled. Scroll down the same file and find the line below. Uncomment it by removing the `#`.
```
#disable_uboot_overlay_video=1
```
4. Save the file and exit.
5. Reboot the Beaglebone by typing the following command:
```
$ sudo reboot
```
6. NOTE! P9_42 is not the standard CS pin for SPI1. Therefore it has to be configured manually upon every boot of the BBB. Type the following command to set up P9_42 for SPI1_CS1.
```
$ sudo config-pin P9_42 spi_cs
```

## Beaglebone SPI Configurations

### SPIDEV
- spidev0.0:    Bus 0, Channel 0
- spidev0.1:    Bus 0, Channel 1
- spidev1.0:    Bus 1, Channel 0
- spidev1.1:    Bus 1, Channel 1 (We will be using this version)

### SPI MODES
- MODE0:    Low at idle, capture on rising clock edge
- MODE1:    Low at idle, capture on falling clock edge
- MODE2:    High at idle, capture on falling clock edge
- MODE3:    High at idle, capture on rising clock edge

# Sources

#### RFM69HCW Library: https://gitlab.com/sedgwickcharles/RFM69

#### SPI Library: https://github.com/derekmolloy/exploringrpi/tree/master/chp09/spiADC_MCP3208/bus

#### GPIO Library: https://github.com/derekmolloy/exploringBB/tree/master/chp06/GPIO


