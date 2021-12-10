

# KeAo - RFM69HCW on Raspberry Pi

This is a documentation of getting the RFM69HCW connected to the Raspberry Pi using the wiringPi library. Note that using the wiringPi library does not use any COSMOS dependencies. It is purely compiled on the Raspberry Pi using g++.

## Getting Started
Note: First make sure the Raspberry Pi Zero is flashed.

### Set up SSH and wireless connection on Raspberry Pi Zero
1. Plug in the microSD card into your computer
2. Navigate to the Boot drive of the microSD card and create a new file called `ssh`. It is important that this file does not have any extensions and that the file is empty.
	- There are many ways to do this, such as creating a file using a terminal. i.e. `touch ssh`
	- You can also use Notepad++ to create a new file and save it in the Boot drive of the microSD card.
3. Then create a new file called `wpa_supplicant.conf`. It is important that this file has the `.conf` extension.
	- Open the file using Notepad++ or Visual Studio Code. DO NOT use Notepad.
	- Insert the following lines in the file:
```
country=US
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1

network={
	ssid="NETWORK_NAME"
	psk="NETWORK_PASSWORD"
	scan_ssid=1
	key_mgmt=WPA-PSK
}
```
Change `NETWORK_NAME` to the name of your network and `NETWORK_PASSWORD` to the password of your network. When the Raspberry Pi boots, this file will be deleted but the network credentials will be saved. That way, your network is not exposed to anyone else who may have access to the Raspberry Pi. Anytime you want to connect to a new network, you have to recreate the file and insert the network name and password. 

**Note**: The Raspberry Pi Zero only supports 2.4 GHz networks. 5G networks will not work.

4. Safely eject the microSD card and insert back into the Raspberry Pi.
5. Connect a cable from your computer to the power port on the Raspberry Pi. You don't need to plug it into the data port. It may take 1-2 minutes for the Raspberry Pi to boot.
6. You can now SSH into the Raspberry Pi using the IP address assigned to it by the router/network.
7. If you plan to SSH into the Raspberry Pi on the computer that it is directly connected to, `ssh pi@raspberrypi` will also work. The default password is `pi`.


### Setting up the Serial Peripheral Interface (SPI)

By default, SPI is disabled on the Raspberry Pi Zero. Follow these steps to enable it:

1. After you SSH into the Raspberry Pi, run `sudo raspi-config`
2. Navigate to `Interfacing Options`
3. Navigate to `P4 SPI`
4. It will prompt you if you would like the SPI interface to be enabled. Select `<Yes>`
5. If it prompts you to restart, proceed with the restart. Then skip to step 8.
6. If not, exit the configuration page by selecting `<Finish>`
7. Run `sudo reboot`
8. Verify that SPI is enabled, run `ls -l /dev/spidev*`
	- You should see the files `/dev/spidev0.0` and `/dev/spidev0.1`
9. Done!
