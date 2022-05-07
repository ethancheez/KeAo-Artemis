#include <iostream>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <unistd.h>


#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 1000000

unsigned char buf[2] = {'a',0};

int main(int argc, char **argv)
{
	wiringPiSetup();
	pinMode(24, OUTPUT);

	int fd = wiringPiSPISetupMode(SPI_CHANNEL, SPI_CLOCK_SPEED, 0);
	if (fd == -1) {
		std::cout << "Failed to init SPI communication.\n";
		return -1;
	}
	std::cout << "SPI communication successfully setup.\n";

	digitalWrite(24, LOW);
	wiringPiSPIDataRW(SPI_CHANNEL, 0x00, 1);
	usleep(100);
	wiringPiSPIDataRW(SPI_CHANNEL, buf, 2);
	usleep(100);
	digitalWrite(24, HIGH);
	std::cout << "Data returned: " << +buf[1] << "\n"; 

	return 0;
}
