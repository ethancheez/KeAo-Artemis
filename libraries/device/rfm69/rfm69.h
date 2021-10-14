#ifndef RFM69_H_INCLUDED
#define RFM69_H_INCLUDED
/**
 * Charles Sedgwick
 * RFM69HCW driver for BeagleBone Black/Green
 * This driver is a port of dltech's generic C driver for the RFM69 which 
 * can be found here: https://github.com/dltech/RFM69.git
 * It also changes based on the LowPowerLab Arduino Driver that can be found
 * here: https://github.com/LowPowerLab/RFM69
 *
 *
 * Original Header:
 ** @file rfm69.h @brief This file contains radiomodule settings, some macro 
 *  for MCU and functions prototypes.
 *
 *  Radiomodule settings broken down into two parts. The first part contains 
 *  basic radiomodule settings (carrier frequency, deviation, bitrate, etc).
 *  The second part consist of radiomodule registers and radiomodule 
 *  configuration bits, which are the same to registers and bits in datasheet.
 *  The first part is sufficient for most applications. And the second part is
 *  useful for detailed configuration of radiomodule. Settings, which 
 *  calculated from parametres from the first part are passed into registers in
 *  the second part. 
*/ 

#include "device/bus/SPIDevice.h"
#include "device/gpio/GPIO.h"

// types of modulation
/// @cond
#define OOK             0x08
#define FSK             0x00
/// @endcond

/*****************************************************************************/
/** @name                       Radiomodule settings                         */
/// @{

#define BITRATE                 9600                ///< bitrate, bit per second
#define CARRIER_FREQ            868250000           ///< carrier frequency in Hz
#define OUTPUT_POWER            13                  ///< output power in dBm (13 dBm max)
#define MAX_POWER_LEVEL         31

#define RX_BW                   65                  ///< bandwidth of the channel filter in kHz
#define RX_BW_AFC               130                 ///< bandwidth of the AFC channel filter in kHz

#define OCP_CURRENT             95                  ///< overcurrent protection maximum current in ma
    
// modulation
#define MODUL_TYPE              FSK                 ///< choose modulation (FSK or OOK)

// FSK parametres
#define DEVIATION               20000               ///< deviation frequency in HZ
#define RISE_FALL_TIME_FSK      50                  ///< rize/fall time FSK (from 10 to 3400)

// OOK parametres
// for peak demodulator
#define OOK_PEAK_THRES_STEP     1                   // size of each decrement of the RSSI threshold in the OOK demodulator in dB (from 0.5 to 6.0)
#define OOK_PEAK_THRESH_DEC     1                   // period of decrement of the RSSI threshold in the OOK demodulator exprecced in times per cheap (from 0.125 to 16)
// for fixed threshold demodulator
#define OOKFIXEDTHRESH          6                   // theshold value in dB

// Packet handler configuration
#define PREAMBLE                0x03                   ///< preamble lenght in bytes
#define SYNC_WORD_SIZE          0x04                   ///< sync word size in bytes (choose 0 if you do not use it)
#define SYNC_WORD               0x753be1ca753be1ca  ///< sync word
#define SYNCTOL                 0x02                   ///< number of tolerated bits in sync word

#define RX_ADDRESS              0x01                ///< radiomodule address
#define BROADCAST_ADDRESS       0x64                ///< broadcast address

#define AUTO_RESTART_RX_DELAY   1                   ///< auto restart rx delay in ms

#define RSSI_THRESH             0xE4                  ///< RSSI threshold value in dBm
#define FIFO_THRESHOLD          0x20                  ///< FIFO threshold value in bytes

#define CUT_OFF_FREQ            0x04                   ///< cut off frecuency expressed in % of RXBW
#define CUT_OFF_FREQ_AFC        0x04                   ///< AFC cut off frecuency expressed in % of RXBW 

/// @}
/*****************************************************************************/

/// @cond
/*****************************************************************************/
/*                           Constants                                       */

#define FXOSC               32000000
#define FSTEP               61
#define PI                  3.14159265359
#define RFM69_BUFFER_SIZE   61
#define CSMA_LIMIT          -90
#define RF69_CSMA_LIMIT_S   1
#define RF69_CSMA_LIMIT_MS  1000
#define RF69_BROADCAST_ADDR 255

#define RFM69_CTL_SENDACK   0x80
#define RFM69_CTL_REQACK    0x40

/*****************************************************************************/

#include "device/rfm69/rfm69_table.h"
#include "device/rfm69/rfm69_registers.h"

/*****************************************************************************/
/*                          Radiomodule registers                            */
#define REGOPMODE_DEF    0x00
#define REGDATAMODUL_DEF  0x02


#define REGBITRATEMSB_DEF   0x03 // value of this register is calculated from the settings above (it is desirable not to change this value)
#define REGBITRATELSB_DEF   0x04 // value of this register is calculated from the settings above (it is desirable not to change this value)

// value of this register is calculated from the settings above (it is desirable not to change this value)
//#define REGFRFMSB_DEF           ( (FRF_CALC(CARRIER_FREQ) & 0xff0000) >> 16 )   
#define REGFRFMSB_DEF   0xE4

// value of this register is calculated from the settings above (it is desirable not to change this value)
//#define REGFRFMID_DEF           ( (FRF_CALC(CARRIER_FREQ) & 0x00ff00) >> 8 )    
#define REGFRFMID_DEF   0xC0

// value of this register is calculated from the settings above (it is desirable not to change this value)
//#define REGFRFLSB_DEF           (  FRF_CALC(CARRIER_FREQ) & 0x0000ff )          
#define REGFRFLSB_DEF   0x00

#define REGAFCCTRL_DEF          ( 0<<(AFCLOWBETAON) )

#define REGLISTEN1_DEF          ( 0x0D )
#define REGLISTEN2_DEF          ( 0x0E )
#define REGLISTEN3_DEF          ( 0x0F )

#define REGPALEVEL_DEF    0x11
#define REGPARAMP_DEF    0x12// value of this register is calculated from the settings above (it is desirable not to change this value)
#define REGOCP_DEF  0x13
#define REGLNA_DEF  0x18

// OOK reqisters
#define REGOOKPEAK_DEF          ( 0x1B )
#define REGOOKAVG_DEF           ( 0x1C )
#define REGOOKFIX_DEF           ( 0x1D )

#define REGAFCFEI_DEF    0x1E

#define REGDIOMAPPING1_DEF      ( DIO0MAP0 | DIO1MAP0 | DIO2MAP2 | DIO3MAP1 )
#define REGDIOMAPPING2_DEF      ( DIO5MAP0 | CLKOUT8M )

#define REGRSSITHRESH_DEF       ( RSSI_THRESH_CALC(RSSI_THRESH) )               // value of this register is calculated from the settings above (it is desirable not to change this value)

// packet handler registers
#define REGPREAMBLEMSB_DEF    0x2C 
#define REGPREAMBLELSB_DEF    0x2D 

#define REGSYNCVALUE1_DEF    0x2D  // attempt to make compatible with RFM12B lib
#define REGSYNCVALUE2_DEF    0x64  // default network id = 100
#define REGSYNCVALUE3_DEF    0x31 
#define REGSYNCVALUE4_DEF    0x32 
#define REGSYNCVALUE5_DEF    0x33 
#define REGSYNCVALUE6_DEF    0x34 
#define REGSYNCVALUE7_DEF    0x35 
#define REGSYNCVALUE8_DEF    0x36 

#define REGPACKETCONFIG1_DEF    ( PACKETFORMAT_FIXED | ENCODING_OFF | CRCON | CRCAUTOCLEAROFF | NODE_BROADCAST_ADDR)
#define REGPAYLOADLENGTH_DEF    66
#define REGNODEADRS_DEF         ( RX_ADDRESS )                                  // value of this register is calculated from the settings above (it is desirable not to change this value)
#define REGBROADCASTADRS_DEF    ( BROADCAST_ADDRESS )                           // value of this register is calculated from the settings above (it is desirable not to change this value)

#define REGAUTOMODES_DEF        ( 0x00 )

#define REGFIFOTHRES_DEF        ( 1<<TXSTARTCOND | (FIFO_THRESHOLD&0x7f) )
#define REGPACKETCONFIG2_DEF    ( INTERPACKETRXDELAY | 1<<AUTORXRESTARTON | 0<<AESON )

#define REGAESKEY1_DEF          ( 0x00 )
#define REGAESKEY2_DEF          ( 0x00 )
#define REGAESKEY3_DEF          ( 0x00 )
#define REGAESKEY4_DEF          ( 0x00 )
#define REGAESKEY5_DEF          ( 0x00 )
#define REGAESKEY6_DEF          ( 0x00 )
#define REGAESKEY7_DEF          ( 0x00 )
#define REGAESKEY8_DEF          ( 0x00 )
#define REGAESKEY9_DEF          ( 0x00 )
#define REGAESKEY10_DEF         ( 0x00 )
#define REGAESKEY11_DEF         ( 0x00 )
#define REGAESKEY12_DEF         ( 0x00 )
#define REGAESKEY13_DEF         ( 0x00 )
#define REGAESKEY14_DEF         ( 0x00 )
#define REGAESKEY15_DEF         ( 0x00 )
#define REGAESKEY16_DEF         ( 0x00 )
/*****************************************************************************/
/// @endcond

/*****************************************************************************/
/** @name                           MCU settings                             */
/// @{
#define CRCOK_PKSent_Line   EXTI_Line2
#define FifoLevel_Line      EXTI_Line1
#define SyncAddr_Line       EXTI_Line0

#define NSS_Port            GPIOA
#define NSS_Pin             GPIO_Pin_3
#define CS_Pin              7

#define EXTI_Port1          GPIO_PortSourceGPIOB
#define EXTI_Port23         GPIO_PortSourceGPIOA


#define EXTI_Pin1           GPIO_PinSource0
#define EXTI_Pin2           GPIO_PinSource1
#define EXTI_Pin3           GPIO_PinSource2

#define RFM69_SYNCADDR_PERIOD   PREAMBLE + SYNC_WORD_SIZE + 1

/// @}
/*****************************************************************************/

// available frequency bands
#define RF69_315MHZ            31 // non trivial values to avoid misconfiguration
#define RF69_433MHZ            43
#define RF69_868MHZ            86
#define RF69_915MHZ            91

/*****************************************************************************/

using namespace exploringBB;

namespace cubesat{

/// This enum describes state of radiomodule, which stored in rfm69_condition variable.
enum RFM69STATE{
    RFM69_SPI_FAILED = 1,   ///< failed to read register through SPI
    RFM69_SLEEP,            ///< radiomodule is in sleep mode
    RFM69_STBY,             ///< radiomodule is in standby mode
    RFM69_RX,               ///< receiver is on (radiomodule waits for the packet)
    RFM69_TX,               ///< transmitter is on (radiomodule transmits data)
    RFM69_NEW_PACK,         ///< the packet received successfully, radiomodule is in sleep mode
    RFM69_SYNTH
};

class RFM69HCW{

public:

    /** @name                   Register access functions                       **/
    RFM69HCW(uint8_t address, uint8_t device, SPIDevice::SPIMODE spi_mode, uint8_t freqBand, uint8_t networkID); /* constructor */
    void rfm69_write(uint8_t address, uint8_t data);
    uint8_t rfm69_read(uint8_t address);
    void rfm69_write_burst(uint8_t address, uint8_t* data, uint8_t ndata);
    void rfm69_read_burst(uint8_t address, uint8_t* data, uint8_t ndata);   

    /** @name                   Initialization functions                        **/
    void rfm69_mcu_init(uint8_t adress, uint8_t device, SPIDevice::SPIMODE spi_mode); /* sets up SPI connection */
    int rfm69_init(uint8_t freqBand, uint8_t networkID); /* connects to radio and sets initial state */

    /** @name       Functions for transmitting or receiving packets             **/
    int rfm69_transmit_start(uint8_t packet_size, uint8_t address);

    void rfm69_receive_start(void);
    int rfm69_receive_small_packet(void);

    void rfm69_setHighPowerRegs(bool state);

    bool rfm69_canSend(void);
    void rfm69_send(uint8_t toAddress, std::string buffer, uint8_t bufferSize, bool requestACK);
    bool rfm69_sendWithRetry(uint8_t toAddress, std::string buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime); // 40ms roundtrip req for 61byte packets
    void rfm69_sendFrame(uint8_t toAddress, std::string buffer, uint8_t bufferSize, bool requestACK, bool sendACK);
    bool rfm69_ACKReceived(uint8_t fromNodeID);
    void rfm69_receiveBegin(void);
    bool rfm69_receiveDone(void);
    bool rfm69_ACKRequested(void);
    void rfm69_sendACK(string buffer, uint8_t bufferSize);

    /** @name               Radiomodule management functions                    **/
    void getData( char *data);
    void setMode(RFM69STATE mode);
    void setCSPin(uint8_t mode);
    void rfm69_select(void);
    void rfm69_unselect(void);
    int getRSSI(int);
    void rfm69_setHighPower(int);
    void rfm69_sleep(void);


    ~RFM69HCW(void);

private:
    RFM69STATE rfm69_condition; /* current state of radio */
    SPIDevice *spi; /* spi connection to radio */

    GPIO *csPin;
    GPIO *resetGPIO;

    uint8_t packet_buffer[RFM69_BUFFER_SIZE];  ///< packet buffer
    uint8_t packet_size;                            ///< packet size

    uint8_t payloadLen;
    uint8_t dataLen;
    uint8_t targetID;
    uint8_t senderID;
    uint8_t rssi;

    uint8_t ACK_RECEIVED;
    uint8_t ACK_REQUESTED;

    uint8_t _address;
};

} /* cubesat namespace */


#endif // RFM69_H_INCLUDED

