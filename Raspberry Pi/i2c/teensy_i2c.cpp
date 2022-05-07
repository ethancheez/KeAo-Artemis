#include <iostream>
#include <wiringPiI2C.h>
#include <string.h>

#define DEVICE_ID 0x08

std::string str = "I am Pi\n";

int main (int argc, char **argv)
{
	// Setup I2C communication
	int fd = wiringPiI2CSetup(DEVICE_ID);
	if (fd == -1) {
		std::cout << "Failed to init I2C communication.\n";
		return -1;
	}
	std::cout << "I2C communication successfully setup.\n";

	while(1) {
		std::getline(std::cin, str);

		if(str.back() != '\n') {
			str += '\n';
		}

		// Send data to arduino
		uint8_t data_to_send = 'a';

		for(int i = 0; i < str.size(); i++) {
			wiringPiI2CWrite(fd, (uint8_t)str[i]);
		}

		std::cout << "Sent data: " << str;

		// Read data from arduino
		char ch;
		std::cout << "Data Received: ";
		while((ch = wiringPiI2CRead(fd)) != '\n') {
			std::cout << ch;
		}
		std::cout << '\n';

	}

	return 0;
}
