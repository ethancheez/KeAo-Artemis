#include <stdbool.h>
#include "device/rfm69/rfm69_registers.h"
#include "device/bus/SPIDevice.h"
//#include "GPIOMap.h"

#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access
#define RF69_MODE_SLEEP         0 // XTAL OFF
#define RF69_MODE_STANDBY       1 // XTAL ON
#define RF69_MODE_SYNTH         2 // PLL ON
#define RF69_MODE_RX            3 // RX MODE
#define RF69_MODE_TX            4 // TX MODE

// available frequency bands
#define RF69_315MHZ            31 // non trivial values to avoid misconfiguration
#define RF69_433MHZ            43
#define RF69_868MHZ            86
#define RF69_915MHZ            91

#define null                  0
#define COURSE_TEMP_COEF    -90 // puts the temperature reading in the ballpark, user can fine tune the returned value
#define RF69_BROADCAST_ADDR   0
#define RF69_CSMA_LIMIT_MS 1000
#define RF69_TX_LIMIT_MS   1000
#define RF69_FSTEP  61.03515625 // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet)

// TWS: define CTLbyte bits
#define RFM69_CTL_SENDACK   0x80
#define RFM69_CTL_REQACK    0x40

#define RFM69_ACK_TIMEOUT   30  // 30ms roundtrip req for 61byte packets

using namespace exploringRPi;

namespace cubesat {

class RFM69 {

public:
    unsigned char initialize(unsigned char freqBand, unsigned short ID, unsigned char networkID, unsigned char intPin, unsigned char rstPin, unsigned char spiBus);
    void setAddress(unsigned short addr);
    void setNetwork(unsigned char networkID);
    bool canSend();
    void send(unsigned short toAddress, const void* buffer, unsigned char bufferSize, bool requestACK);
    bool sendWithRetry(unsigned short toAddress, const void* buffer, unsigned char bufferSize, unsigned char retries, unsigned char retryWaitTime);
    bool receiveDone();
    bool ACKReceived(unsigned short fromNodeID);
    bool ACKRequested();
    void sendACK(const void* buffer, unsigned char bufferSize);
    int getFrequency(); //-------//
    void setFrequency(unsigned int freqHz);
    void encrypt(unsigned char* key);
    short readRSSI(bool forceTrigger); // *current* signal strength indicator; e.g. < -90dBm says the frequency channel is free + ready to transmit
    void spyMode(bool onOff); //-------//
    void setHighPower(bool onOFF); // has to be called after initialize() for RFM69HW
    void setPowerLevel(unsigned char level); // reduce/increase transmit power level
    void sleep();
    unsigned char readTemperature(unsigned char calFactor); // get CMOS temperature (8bit)
    void rcCalibration();
    void sendFrame(unsigned short toAddress, const void* buffer, unsigned char bufferSize, bool requestACK, bool sendACK);
    void receiveBegin();
    void setMode(unsigned char mode);
    void setHighPowerRegs(bool onOff);
    void interruptHandler();
    unsigned char readReg(unsigned char addr);
    void writeReg(unsigned char addr, unsigned char val);
    //void readAllRegs();
    //bool shutdown();
    static void isr0();

private:
    SPIDevice *spi;

    unsigned char _powerLevel;
    bool _isRFM69HW;
    unsigned short _address;
    unsigned char PAYLOADLEN;
    unsigned short SENDERID;
    unsigned short TARGETID;
    bool _spyMode;
    unsigned char DATALEN;
    unsigned char ACK_REQUESTED;
    unsigned char ACK_RECEIVED;
    short RSSI;
    unsigned char DATA[RF69_MAX_DATA_LEN+1];
    unsigned char _intPin;


};

}
