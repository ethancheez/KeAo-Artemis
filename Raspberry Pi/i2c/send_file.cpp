#include <iostream>
#include <wiringPiI2C.h>
#include <string.h>
#include <fstream>
#include <unistd.h>

#define DEVICE_ID 0x08
int teensy_device;

std::string str = "I am Pi\n";

void send_to_teensy_i2c(std::string str);

int main (int argc, char **argv)
{
	// Setup I2C communication
	teensy_device = wiringPiI2CSetup(DEVICE_ID);
	if (teensy_device == -1) {
		std::cout << "Failed to init I2C communication.\n";
		return -1;
	}
	std::cout << "I2C communication successfully setup.\n";

	// Open File
	std::cout << "File to Send: ";
	std::getline(std::cin, str);
	std::ifstream fs;
	fs.open(str, std::fstream::in);

	while(!fs.is_open()) {
		std::cout << "File not found" << std::endl;
		std::cout << "File to Send: ";
		std::getline(std::cin, str);
		fs.open(str, std::fstream::in);
	}

	// Tell Teensy to create/open file
	str = "CMD: CREATE FILE " + str + '\n';
	send_to_teensy_i2c(str);
	usleep(1000);

	while(std::getline(fs, str)) {
		str += '\n';

		// Send data to Teensy
		send_to_teensy_i2c(str);

		std::cout << "Sent data: " << str;

		// Read data from Teensy
		char ch;
		std::cout << "Data Received: ";
		while((ch = wiringPiI2CRead(teensy_device)) != '\n') {
			std::cout << ch;
		}
		std::cout << '\n';

	}

	// Tell Teensy to close file
	//str = "CMD: CLOSE FILE\n";
	//send_to_teensy_i2c(str);
	wiringPiI2CWrite(teensy_device, (uint8_t)EOF);

	fs.close();
	return 0;
}

void send_to_teensy_i2c(std::string str) {
	for(int i = 0; i < str.size(); i++) {
		wiringPiI2CWrite(teensy_device, (uint8_t)str[i]);
	}
}
