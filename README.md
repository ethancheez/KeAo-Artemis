# KeAo COSMOS

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

3. In order to use the SPI1 pins, the HDMI port MUST be disabled. Scroll down the same file and find the line below. Uncomment it by removing the `#`.
```
#disable_uboot_overlay_video=1
```
Save the file and exit.

4. The firmware to enable SPI1 pins on boot does not include P9_42. We need to create a service to configure this pin upon boot. Go back to your home directory.
```
$ cd
```
You can create a new folder to hold your scripts.
```
$ mkdir scripts
```
Navigate to that folder and create a new file called `init_spi.sh` and copy/paste the following into the file:
```
#!/bin/sh -e

# Configure P9_42 SPI1 Pin
config-pin P9_42 spi_cs

exit 0
```
Save and exit.

5. Make sure that the shell script is executable by typing the following command:
```
$ sudo chmod +x init_spi.sh
```

6. Next, we need to setup the CAN interfaces so the BBB will configure across power cycles. Open the `/etc/network/interfaces` file:
```
$ sudo nano /etc/network/interfaces
```
Copy/paste the following to the bottom of the file:
```
auto can0
iface can0 inet manual
pre-up /sbin/ip link set $IFACE type can bitrate 500000 restart-ms 10 listen-only on
pre-up /sbin/ip link set $IFACE txqueuelen 1000
up /sbin/ip link set up $IFACE
down /sbin/ip link set down $IFACE

auto can1
iface can1 inet manual
pre-up /sbin/ip link set $IFACE type can bitrate 500000 restart-ms 10
pre-up /sbin/ip link set $IFACE txqueuelen 1000
up /sbin/ip link set up $IFACE
down /sbin/ip link set down $IFACE
```
Save the file and exit.

7. Create a service to be run by systemd on startup. Navigate to the `/etc/systemd/system` folder. Create a new file:
```
$ sudo touch init_spi.service
```
Copy/paste the following into the file (you may need to use `sudo` to edit the file):
```
[Unit]
Description=Setup for BBB SPI P9_42 pin

[Service]
Type=simple
ExecStart=/bin/bash /home/debian/scripts/init_spi.sh

[Install]
WantedBy=multi-user.target
```
Save the file and exit.

8. Issue the following commands to start the service:
```
$ sudo systemctl start init_spi.service
$ sudo systemctl enable init_spi.service
```
Verify that the service was started:
```
$ sudo systemctl status init_spi.service
```

9. Reboot the Beaglebone by typing the following command:
```
$ sudo reboot now
```

10. To verify that the SPI pin was configured properly, enter the following command:
```
$ config-pin -q P9_42
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


