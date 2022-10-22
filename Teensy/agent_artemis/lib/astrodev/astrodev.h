#ifndef _ASTRODEVLIB_H
#define _ASTRODEVLIB_H 

#include <stdint.h>

#define ASTRODEV_HEADER_SIZE 8
#define ASTRODEV_PAYLOAD_TRAILER_SIZE 2
#define ASTRODEV_OVERHEAD_SIZE_NO_PAYLOAD (ASTRODEV_HEADER_SIZE)
#define ASTRODEV_OVERHEAD_SIZE_WITH_PAYLOAD (ASTRODEV_HEADER_SIZE + ASTRODEV_PAYLOAD_TRAILER_SIZE)

#define ASTRODEV_SYNC0 'H'
#define ASTRODEV_SYNC1 'e'

#define ASTRODEV_COMMAND 0x10
#define ASTRODEV_RESPONSE 0x20

// Command List
#define ASTRODEV_NOOP 0x01
#define ASTRODEV_RESET 0x02
#define ASTRODEV_TRANSMIT 0x03
#define ASTRODEV_RECEIVE 0x04
#define ASTRODEV_GETTCVCONFIG 0x05
#define ASTRODEV_SETTCVCONFIG 0x06
#define ASTRODEV_TELEMETRY 0x07
#define ASTRODEV_FLASH 0x08
#define ASTRODEV_RFCONFIG 0x09
#define ASTRODEV_BEACONDATA 0x10
#define ASTRODEV_BEACONCONFIG 0x11
#define ASTRODEV_FIRMWAREREV 0x12
#define ASTRODEV_DIOKEY 0x13
#define ASTRODEV_FIRMWAREUPDATE 0x14
#define ASTRODEV_FIRMWAREPACKET 0x15
#define ASTRODEV_WRITE_KEY_A_128 0x16
#define ASTRODEV_WRITE_KEY_B_128 0x17
#define ASTRODEV_WRITE_KEY_A_256 0x18
#define ASTRODEV_WRITE_KEY_B_256 0x19
#define ASTRODEV_FASTSETPA 0x20
#define ASTRODEV_GET_RTC 0x41
#define ASTRODEV_SET_RTC 0x42
#define ASTRODEV_ALARM_RTC 0x43

#define ASTRODEV_16BYTE_OA_KEY "ABCDEFGHIJKLMNOP"

#define ASTRODEV_RF_BAUD(rate) (rate?4800<<rate:1200)
#define ASTRODEV_UART_BAUD(rate) (9600<<rate)

enum
	{
	ASTRODEV_BAUD_RATE_9600,
	ASTRODEV_BAUD_RATE_19200,
	ASTRODEV_BAUD_RATE_38400,
	ASTRODEV_BAUD_RATE_76800,
	ASTRODEV_BAUD_RATE_115200
	};

enum
	{
	ASTRODEV_RF_BAUD_RATE_1200,
	ASTRODEV_RF_BAUD_RATE_9600,
	ASTRODEV_RF_BAUD_RATE_19200,
    ASTRODEV_RF_BAUD_RATE_38400,
    ASTRODEV_RF_BAUD_RATE_57600,
    ASTRODEV_RF_BAUD_RATE_115200,
    };

enum
	{
	ASTRODEV_MODULATION_GFSK,
	ASTRODEV_MODULATION_AFSK,
	ASTRODEV_MODULATION_BPSK
	};

// OA Commands
#define ASTRODEV_TELEMETRY_DUMP_COMMAND 0x30
#define ASTRODEV_PING_RETURN_COMMAND 0x31
#define ASTRODEV_CODE_UPLOAD_COMMAND 0x32
#define ASTRODEV_RADIO_RESET_COMMAND 0x33
#define ASTRODEV_PIN_TOGGLE_COMMAND 0x34

#define ASTRODEV_ID 1
#define ASTRODEV_BAUD 9600
#define ASTRODEV_BITS 8
#define ASTRODEV_PARITY 0
#define ASTRODEV_STOPBITS 1

#define ASTRODEV_MTU 254

typedef struct
{
    uint8_t interface_baud_rate;    //Radio Interface Baud Rate (9600=0x00)
    uint8_t tx_power_amp_level;     //Tx Power Amp level (min = 0x00 max = 0xFF)
    uint8_t rx_rf_baud_rate;        //Radio RX RF Baud Rate (9600=0x00)
    uint8_t tx_rf_baud_rate;        //Radio TX RF Baud Rate (9600=0x00)
    uint8_t rx_modulation;          //(0x00 = GFSK);
    uint8_t tx_modulation;          //(0x00 = GFSK);
    uint32_t rx_freq;               //Channel Rx Frequency (ex: 45000000)
    uint32_t tx_freq;               //Channel Tx Frequency (ex: 45000000)
    unsigned char source[6];        //AX25 Mode Source Call Sign (default NOCALL)
    unsigned char destination[6];   //AX25 Mode Destination Call Sign (default CQ)
    uint16_t tx_preamble;           //AX25 Mode Tx Preamble Byte Length (0x00 = 20 flags)
    uint16_t tx_postamble;          //AX25 Mode Tx Postamble Byte Length (0x00 = 20 flags)
    uint16_t function_config;       //Radio Configuration Discrete Behaviors
    uint16_t function_config2;      //Radio Configuration Discrete Behaviors 2
} RADIO_CONFIGURATION_TYPE;
#define RADIO_CONFIG_SIZE 34        //sizeof(RADIO_CONFIGURATION_TYPE)

typedef struct
{
    uint8_t front_end_level;        //0 to 63 Value
    uint8_t tx_power_amp_level;     //0 to 255 value, non-linear
    uint32_t tx_frequency_offset;   //Up to 20 kHz
    uint32_t rx_frequency_offset;   //Up to 20 kHz
    uint8_t tx_frequency_deviation; //Set for your baud rate options: 0 (2.7 kHz),1 (5.4 kHz),2 (10.8 kHz),3 (21.6 kHz),4 (43.2 kHz) CAUTION
    uint8_t rx_frequency_deviation; //N/A for release 3.10
    uint8_t pre_transmit_delay;     //Delay in tens of milliseconds. Default 1 second = 100;
    uint8_t post_transmit_delay;    //Delay in tens of milliseconds. Default 0 (NOT IMPLEMENTED USE POSTAMBLE)
} RADIO_RF_CONFIGURATION_TYPE;
#define RF_CONFIG_SIZE 14

typedef struct
{
    uint8_t beacon_interval;        //value of 0 is off, 2.5 sec delay per LSB
} RADIO_BEACON_CONFIGURATION_TYPE;
#define BEACON_CONFIG_SIZE 1

typedef struct telem_type
{
    uint16_t op_counter;
    int16_t msp430_temp;
    uint8_t time_count[3];
    uint8_t rssi;
    uint32_t bytes_received;
    uint32_t bytes_transmitted;
    uint8_t rssi_lastpacket;
    uint8_t rtc_alarm_flag;
} TELEMETRY_STRUCTURE_type;

typedef struct RTC_type
{
    unsigned short int year;    // Year = 0x2021
    unsigned char mon;          // Month = 0x10 = October
    unsigned char day;          // Day = 0x07 = 7th
    unsigned char dow;          // Day of week = 0x05 = Friday
    unsigned char hour;         // Hour = 0x11
    unsigned char min;          // Minute = 0x59
    unsigned char sec;          // Seconds = 0x30
    unsigned char alarm_dow;    // RTC Day of week alarm = 0x2
    unsigned char alarm_day;    // RTC Day Alarm = 0x20
    unsigned char alarm_hour;   // RTC Hour Alarm
    unsigned char alarm_min;    // RTC Minute Alarm
} RTC_STRUCTURE_type;


#endif // _ASTRODEVLIB_H 
