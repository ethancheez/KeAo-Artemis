#include <iostream>
#include <unistd.h>
#include <chrono>
#include "support/configCosmos.h"
#include "device/rfm69/rfm69.h"
#include "device/gpio/GPIO.h"

using namespace std;
using namespace exploringBB;

namespace cubesat {

/** 
    @function
    Constructor
*/
RFM69HCW::RFM69HCW(uint8_t address, uint8_t device, SPIDevice::SPIMODE spi_mode, uint8_t freqBand, uint8_t networkID){
    this->dataLen = 0;
    this->senderID = 0;
    this->targetID = 0;
    this->payloadLen = 0;
    this->ACK_REQUESTED = 0;
    this->ACK_RECEIVED = 0;
    this->rssi = 0;

    this->csPin = new GPIO(CS_Pin);
    this->resetGPIO = new GPIO(CS_Pin);

    /* call rfm69_init() */
    rfm69_mcu_init(address, device, spi_mode);
    rfm69_init(freqBand, networkID);

    this->_address = 1;
}

/**
    @function
    This function implements MCU initialisation, i.e. initialisaton of SPI
    interface, interrupts, GPIOs, etc.
    TODO: replace code with SPIDevice calls
*/
void RFM69HCW::rfm69_mcu_init(uint8_t address, uint8_t device, SPIDevice::SPIMODE spi_mode)
{
//  SPI initialisation
    this->spi = new SPIDevice(address,device,spi_mode);
}

/**
    @function
    This function performs radiomodule initialisation. It writes radiomodule
    register values via SPI, which were defined and calculated in file rfm69.h.
    It also checks the SPI interface, if it doesn't work, it sets the @ref
    RFM69_SPI_FAILED state.
    @return 0 if initialisation completed successfully, and -1 otherwise
*/
int RFM69HCW::rfm69_init(uint8_t freqBand, uint8_t networkID){
    const uint8_t CONFIG[][2] =
      {
        /* 0x01 */ { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
        /* 0x02 */ { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
        /* 0x03 */ { REG_BITRATEMSB, RF_BITRATEMSB_55555}, // default: 4.8 KBPS
        /* 0x04 */ { REG_BITRATELSB, RF_BITRATELSB_55555},
        /* 0x05 */ { REG_FDEVMSB, RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
        /* 0x06 */ { REG_FDEVLSB, RF_FDEVLSB_50000},

        /* 0x07 */ { REG_FRFMSB, (uint8_t) (freqBand==RF69_315MHZ ? RF_FRFMSB_315 : (freqBand==RF69_433MHZ ? RF_FRFMSB_433 : (freqBand==RF69_868MHZ ? RF_FRFMSB_868 : RF_FRFMSB_915))) },
        /* 0x08 */ { REG_FRFMID, (uint8_t) (freqBand==RF69_315MHZ ? RF_FRFMID_315 : (freqBand==RF69_433MHZ ? RF_FRFMID_433 : (freqBand==RF69_868MHZ ? RF_FRFMID_868 : RF_FRFMID_915))) },
        /* 0x09 */ { REG_FRFLSB, (uint8_t) (freqBand==RF69_315MHZ ? RF_FRFLSB_315 : (freqBand==RF69_433MHZ ? RF_FRFLSB_433 : (freqBand==RF69_868MHZ ? RF_FRFLSB_868 : RF_FRFLSB_915))) },

        // looks like PA1 and PA2 are not implemented on RFM69W/CW, hence the max output power is 13dBm
        // +17dBm and +20dBm are possible on RFM69HW
        // +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1**)
        // +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
        // +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high power PA settings (section 3.3.7 in datasheet)
        ///* 0x11 */ { REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},
        ///* 0x13 */ { REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95 }, // over current protection (default is 95mA)

        // RXBW defaults are { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5} (RxBw: 10.4KHz)
        /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
        //for BR-19200: /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3 },
        /* 0x25 */ { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
        /* 0x26 */ { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
        /* 0x28 */ { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
        /* 0x29 */ { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
        ///* 0x2D */ { REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE } // default 3 preamble bytes 0xAAAAAA
        /* 0x2E */ { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
        /* 0x2F */ { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
        /* 0x30 */ { REG_SYNCVALUE2, networkID }, // NETWORK ID
        //* 0x31 */ { REG_SYNCVALUE3, 0xAA },
        //* 0x31 */ { REG_SYNCVALUE4, 0xBB },
        /* 0x37 */ { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
        /* 0x38 */ { REG_PAYLOADLENGTH, 66 }, // in variable length mode: the max frame size, not used in TX
        ///* 0x39 */ { REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
        /* 0x3C */ { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
        /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_OFF | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        //for BR-19200: /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        /* 0x6F */ { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
        {255, 0}
      };

    // Reset radio
    cout << "Resetting Radio . . .";
    this->resetGPIO->setDirection(OUTPUT);
    this->resetGPIO->setValue(HIGH);
    COSMOS_SLEEP(0.2); // wait 0.2s
    this->resetGPIO->setValue(LOW);
    COSMOS_SLEEP(0.2); // wait 0.2s
    cout << " Radio RESET!" << endl;

    setCSPin(HIGH);

    // Ensure that the radio is able to read and write through SPI
    for(int cycle = 0; rfm69_read(REG_SYNCVALUE1) != 0xaa; cycle++) {
        if(cycle > 100) {
            printf("SPI was not properly initialized. Exiting . . .\n");
            exit (0);
        }
        rfm69_write(REG_SYNCVALUE1, 0xaa);
    }

    for(int cycle = 0; rfm69_read(REG_SYNCVALUE2) != 0x55; cycle++) {
        if(cycle > 100) {
            printf("SPI was not properly initialized. Exiting . . .\n");
            exit (0);
        }
        rfm69_write(REG_SYNCVALUE2, 0x55);
    }

    //  RFM69 initialization
    for (int i = 0; CONFIG[i][0] != 255; i++) {
        rfm69_write(CONFIG[i][0], CONFIG[i][1]);
    }

    rfm69_setHighPower(1);
    setMode(RFM69_STBY);

    while ((rfm69_read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady

    // SPI check
    int ret = rfm69_read(REG_RXBW);
    if ( ret != (RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2) ) {
        this->rfm69_condition = RFM69_SPI_FAILED;
        cerr<<"RFM69HCW::rfm69_init->SPI failed. ret: '"<<ret<<"'"<<endl;
        return -1;
    }else{
        cout << endl << "-------------------------------------------" << endl;
        cout << "RADIO SUCCESSFULLY INITIALIZED" << endl;
        cout << "-------------------------------------------" << endl;
        return 0;
    }
}

void RFM69HCW::setMode(RFM69STATE mode) {

  if(this->rfm69_condition == mode)
      return;

  switch (mode) {
    case RFM69_TX:
      rfm69_write(REG_OPMODE, (rfm69_read(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
      rfm69_setHighPowerRegs(true);
      break;
    case RFM69_RX:
      rfm69_write(REG_OPMODE, (rfm69_read(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
      rfm69_setHighPowerRegs(false);
      break;
    case RFM69_SYNTH:
      rfm69_write(REG_OPMODE, (rfm69_read(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
      break;
    case RFM69_STBY:
      rfm69_write(REG_OPMODE, (rfm69_read(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
      break;
    case RFM69_SLEEP:
      rfm69_write(REG_OPMODE, (rfm69_read(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
      break;
    default:
      return;
    }

  while(this->rfm69_condition == RFM69_SLEEP && (rfm69_read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00);

  this->rfm69_condition = mode;
}
  

/** 
    @function
    Write radiomodule register via SPI.
    @param address - register address
    @param data - register value 
*/
void RFM69HCW::rfm69_write(uint8_t address, uint8_t data)
{

    if( this->spi == NULL ){
        cerr<<"RFM69HCW::rfm69_write->SPI not set"<<endl;
        return;
    }
    rfm69_select();
    if( this->spi->writeRegister(address | 0x80, data) < 0 ){
        cerr<<"RFM69HCW::rfm69_write->Error writing to register "<<address<<endl;
    }
    rfm69_unselect();
}

/** 
    @function
    Read radiomodule register via SPI
    TODO: Modify with SPIDevice calls
    @param address - register address
    @return register value 
*/
uint8_t RFM69HCW::rfm69_read(uint8_t address)
{
    uint8_t data = 0x00;

    rfm69_select();
    data =  this->spi->readRegister( address & 0x7f );
    //cout << "The value that was received is: " << data << endl;

    rfm69_unselect();
	return data;
}



int RFM69HCW::getRSSI(int forceTrigger){
  int rssi = 0;
  if (forceTrigger == 1) {
    //RSSI trigger not needed if DAGC is in continuous mode
    rfm69_write(REG_RSSICONFIG, RF_RSSI_START);
    while ((rfm69_read(REG_RSSICONFIG) & RF_RSSI_DONE) == 0x00); // Wait for RSSI_Ready
  }
  rssi = -rfm69_read(REG_RSSIVALUE);
  rssi >>= 1;
  rssi += 20;
  return rssi;

}

bool RFM69HCW::rfm69_sendWithRetry(uint8_t toAddress, string buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime) {
    for(uint8_t attempt = 1; attempt <= retries; attempt++) {
        cout << "ACK ATTEMPT #" << (int)attempt << endl;
        rfm69_send(toAddress, buffer, bufferSize, true);

        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        while( elapsed_seconds.count() < retryWaitTime ) {
            if(rfm69_ACKReceived(toAddress))
                return true;
            end = std::chrono::steady_clock::now();
            elapsed_seconds = end - start;
        }
    }
    return false;
}

bool RFM69HCW::rfm69_canSend(void) {
    if(this->rfm69_condition == RFM69_STBY) {
        rfm69_receiveBegin();
        return true;
    }
    if(this->rfm69_condition == RFM69_RX && this->payloadLen == 0 && getRSSI(0) < CSMA_LIMIT) {
        setMode(RFM69_STBY);
        return true;
    }
    //cout << "CANNOT SEND" << endl;
    return false;
}

void RFM69HCW::rfm69_send(uint8_t toAddress, string buffer, uint8_t bufferSize, bool requestACK) {
    rfm69_write(REG_PACKETCONFIG2, (rfm69_read(REG_PACKETCONFIG2) & 0xfb) | RF_PACKET2_RXRESTART);

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    while(!rfm69_canSend() /*&& elapsed_seconds.count() < RF69_CSMA_LIMIT_S*/) {
        rfm69_receiveDone();
        end = std::chrono::steady_clock::now();
        elapsed_seconds = end - start;
    }

    rfm69_sendFrame(toAddress, buffer, bufferSize, requestACK, false);
    //cout << "test" << endl;
}

void RFM69HCW::rfm69_sendFrame(uint8_t toAddress, string buffer, uint8_t bufferSize, bool requestACK, bool sendACK) {
    setMode(RFM69_STBY); // turn off receiver to prevent reception while filling fifo

    while ((rfm69_read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) { // wait for ModeReady
        cout << "READ1: " << rfm69_read(REG_IRQFLAGS1) << "   result = " << (rfm69_read(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) << endl;
    }

    rfm69_write(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Packet Sent"
    if (bufferSize > RFM69_BUFFER_SIZE) bufferSize = RFM69_BUFFER_SIZE;

    // control byte
    uint8_t CTLbyte = 0x00;
    if (sendACK)
        CTLbyte = RFM69_CTL_SENDACK;
    else if (requestACK)
        CTLbyte = RFM69_CTL_REQACK;

    // write to FIFO
    rfm69_select();
    this->spi->write(0x80 | REG_FIFO);
    this->spi->write(bufferSize + 3);
    this->spi->write(toAddress);
    this->spi->write(this->_address);
    this->spi->write(CTLbyte);

    for(uint8_t i = 0; i < bufferSize; i++) {
        this->spi->write(buffer[i]);
    }
    rfm69_unselect();

    // no need to wait for transmit mode to be ready since its handled by the radio
    setMode(RFM69_TX);
    //while (RFM69_ReadDIO0Pin()) == 0 && !Timeout_IsTimeout1()); // wait for DIO0 to turn HIGH signalling transmission finish

    while( (rfm69_read(REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT) == 0x00) { // wait for ModeReady
        cout << "READ2: " << rfm69_read(REG_IRQFLAGS2) << "   result = " << (rfm69_read(REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT) << endl;
    }
    setMode(RFM69_STBY);
}

bool RFM69HCW::rfm69_ACKReceived(uint8_t fromNodeID) {
    if (rfm69_receiveDone())
        return (this->senderID == fromNodeID || fromNodeID == RF69_BROADCAST_ADDR) && ACK_RECEIVED;
    return false;
}

void RFM69HCW::rfm69_sendACK(string buffer, uint8_t bufferSize) {
    this->ACK_REQUESTED = 0;   // TWS added to make sure we don't end up in a timing race and infinite loop sending Acks
    uint8_t sender = senderID;
    int16_t l_rssi = rssi; // save payload received RSSI value
    rfm69_write(REG_PACKETCONFIG2, (rfm69_read(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    while (!rfm69_canSend() && elapsed_seconds.count() < RF69_CSMA_LIMIT_S) {
        rfm69_receiveDone();
        end = std::chrono::steady_clock::now();
        elapsed_seconds = end - start;
    }
    this->senderID = sender;    // TWS: Restore senderID after it gets wiped out by RFM69_receiveDone()
    rfm69_sendFrame(sender, buffer, bufferSize, false, true);
    this->rssi = l_rssi; // restore payload RSSI
}

void RFM69HCW::rfm69_receiveBegin(void) {
    //cout << "receiveBegin" << endl;
    this->dataLen = 0;
    this->senderID = 0;
    this->targetID = 0;
    this->payloadLen = 0;
    this->ACK_RECEIVED = 0;
    this->ACK_REQUESTED = 0;
    this->rssi = 0;

    if(rfm69_read(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY) {
        rfm69_write(REG_PACKETCONFIG2, (rfm69_read(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
    }

    rfm69_write(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01); // set DIO0 to "PAYLOADREADY" in receive mode
    setMode(RFM69_RX);
}

bool RFM69HCW::rfm69_receiveDone(void) {
    //cout << "receiveDone" << endl;
    if (this->rfm69_condition == RFM69_RX && this->payloadLen > 0) {
        setMode(RFM69_STBY); // enables interrupts
        return true;
    }

    if(rfm69_read(REG_IRQFLAGS1) & RF_IRQFLAGS1_TIMEOUT) {
        rfm69_write(REG_PACKETCONFIG2, (rfm69_read(REG_PACKETCONFIG2) & 0xfb) | RF_PACKET2_RXRESTART);
    }
    else if (this->rfm69_condition == RFM69_RX) {
        return false;
    }
    rfm69_receiveBegin();
    return false;
}

void RFM69HCW::rfm69_setHighPower(int onOff) {
    if(onOff) {
        rfm69_write(REG_OCP, RF_OCP_OFF);
        rfm69_write(REG_PALEVEL, (rfm69_read(REG_PALEVEL) & 0x1f) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON);
    }
    else {
        rfm69_write(REG_OCP, RF_OCP_ON);
        rfm69_write(REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | MAX_POWER_LEVEL);
    }
}

void RFM69HCW::rfm69_setHighPowerRegs(bool state) {
    rfm69_write(REG_TESTPA1, state ? 0x5D : 0x55);
    rfm69_write(REG_TESTPA2, state ? 0x7C : 0x70);
}

void RFM69HCW::setCSPin(uint8_t mode) {
    this->csPin->setDirection(OUTPUT);
    switch(mode) {
    case HIGH:
        this->csPin->setValue(HIGH);
        break;
    case LOW:
        this->csPin->setValue(LOW);
        break;
    default:
        break;
    }
    COSMOS_SLEEP(0.1);
}

// select the RFM69 transceiver (save SPI settings, set CS low)
void RFM69HCW::rfm69_select(void)  {
  setCSPin(LOW);
}

// unselect the RFM69 transceiver (set CS high, restore SPI settings)
void RFM69HCW::rfm69_unselect(void) {
  setCSPin(HIGH);
}

//put transceiver in sleep mode to save battery - to wake or resume receiving just call RFM69_receiveDone()
void RFM69HCW::rfm69_sleep() {
  setMode(RFM69_SLEEP);
}

/**
    @function
    Destructor
*/
RFM69HCW::~RFM69HCW(){
    delete this->csPin;
    delete this->resetGPIO;

    this->spi->close();
    delete this->spi;
}

} /* cubesat namespace */

