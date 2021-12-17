#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include "support/configCosmos.h"
#include <stdbool.h>
#include <stdio.h>
#include "device/rfm69/rfm69.h"

using namespace std;

namespace cubesat {

static unsigned char _mode;
static bool _haveData;

unsigned char RFM69::initialize(unsigned char freqBand, unsigned short ID, unsigned char networkID, unsigned char intPin, unsigned char rstPin, unsigned char spiBus)
{
    _powerLevel = 31;
    _isRFM69HW = 1;
    _mode = RF69_MODE_RX;
    //_address;
    _haveData = 0;
    PAYLOADLEN = 0;
    _intPin=intPin;
    wiringPiSetupPhys();
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);
    delay(100);
    //COSMOS_SLEEP(0.1);
    digitalWrite(rstPin, LOW);
    delay(100);
    //COSMOS_SLEEP(0.1);
    wiringPiSPISetup(spiBus, 1000000);
    const unsigned char CONFIG[][2] =
    {
        /* 0x01 */ { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
        /* 0x02 */ { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
        /* 0x03 */ { REG_BITRATEMSB, RF_BITRATEMSB_50000}, // default: 4.8 KBPS
        /* 0x04 */ { REG_BITRATELSB, RF_BITRATELSB_50000},
        /* 0x05 */ { REG_FDEVMSB, RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
        /* 0x06 */ { REG_FDEVLSB, RF_FDEVLSB_50000},

        /* 0x07 */ { REG_FRFMSB, (unsigned char) (freqBand==RF69_315MHZ ? RF_FRFMSB_315 : (freqBand==RF69_433MHZ ? RF_FRFMSB_433 : (freqBand==RF69_868MHZ ? RF_FRFMSB_868 : RF_FRFMSB_915))) },
        /* 0x08 */ { REG_FRFMID, (unsigned char) (freqBand==RF69_315MHZ ? RF_FRFMID_315 : (freqBand==RF69_433MHZ ? RF_FRFMID_433 : (freqBand==RF69_868MHZ ? RF_FRFMID_868 : RF_FRFMID_915))) },
        /* 0x09 */ { REG_FRFLSB, (unsigned char) (freqBand==RF69_315MHZ ? RF_FRFLSB_315 : (freqBand==RF69_433MHZ ? RF_FRFLSB_433 : (freqBand==RF69_868MHZ ? RF_FRFLSB_868 : RF_FRFLSB_915))) },

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
        ///* 0x39 */ { REG_NODEADRS, ID }, // turned off because we're not using address filtering
        /* 0x3C */ { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
        /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        //for BR-19200: /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
        /* 0x6F */ { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
        {255, 0}
    };

    //Verify chip syncing
    unsigned int start = millis();
    unsigned char timeout = 50;

    cout << endl << "Check if Radio Connected Through SPI:" << endl;
    do writeReg(REG_SYNCVALUE1, 0xAA); while (readReg(REG_SYNCVALUE1) != 0xaa && millis()-start < timeout);
    if(millis()-start > timeout) return false;
    start = millis();
    do writeReg(REG_SYNCVALUE2, 0x55); while (readReg(REG_SYNCVALUE2) != 0x55 && millis()-start < timeout);
    if(millis()-start > timeout) return false;
    cout << "Finished Check: SUCCESS" << endl << endl;

    unsigned char i;
    for (i = 0; CONFIG[i][0] != 255; i++)
        writeReg(CONFIG[i][0], CONFIG[i][1]);

    encrypt(0);

    setHighPower(_isRFM69HW); // called regardless if it's a RFM69W or RFM69HW
    setMode(RF69_MODE_STANDBY);
    start = millis();
    while (((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) && millis()-start < timeout); // wait for ModeReady
    if (millis()-start >= timeout)
        return false;
    wiringPiISR(intPin, INT_EDGE_RISING, isr0);
    _address = ID;
    return true;
}
void RFM69::setAddress(unsigned short addr)
{
    _address = addr;
    writeReg(REG_NODEADRS, _address); //unused in packet mode
}
void RFM69::setNetwork(unsigned char networkID)
{
    writeReg(REG_SYNCVALUE2, networkID);
}
bool RFM69::canSend()
{
    if (_mode == RF69_MODE_RX && PAYLOADLEN == 0 && readRSSI(0) < CSMA_LIMIT) // if signal stronger than -100dBm is detected assume channel activity
    {
        setMode(RF69_MODE_STANDBY);
        return true;
    }
    return false;
}
void RFM69::send(unsigned short toAddress, const void* buffer, unsigned char bufferSize, bool requestACK)
{
    writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
    unsigned int now = millis();
    while (!canSend() && millis() - now < RF69_CSMA_LIMIT_MS) receiveDone();

    sendFrame(toAddress, buffer, bufferSize, requestACK, false);
}
bool RFM69::sendWithRetry(unsigned short toAddress, const void* buffer, unsigned char bufferSize, unsigned char retries, unsigned char retryWaitTime)
{
    unsigned int sentTime;
    unsigned char i;
    for (i = 0; i <= retries; i++)
    {
        send(toAddress, buffer, bufferSize, true);
        sentTime = millis();
        while (millis() - sentTime < retryWaitTime)
            if (ACKReceived(toAddress)) return true;
    }
    return false;
}
bool RFM69::receiveDone()
{
    if (_haveData)
    {
        _haveData = false;
        interruptHandler();
    }
    if (_mode == RF69_MODE_RX && PAYLOADLEN > 0)
    {
        setMode(RF69_MODE_STANDBY); // enables interrupts
        return true;
    }
    else if (_mode == RF69_MODE_RX) // already in RX no payload yet
    {
        return false;
    }
    receiveBegin();
    return false;
}
bool RFM69::ACKReceived(unsigned short fromNodeID)
{
    if (receiveDone())
    {
        return (SENDERID == fromNodeID || fromNodeID == RF69_BROADCAST_ADDR) && ACK_RECEIVED;
    }
    return false;
}
bool RFM69::ACKRequested()
{
    return ACK_REQUESTED && (TARGETID == _address);
}
void RFM69::sendACK(const void* buffer, unsigned char bufferSize)
{
    ACK_REQUESTED = 0;   // TWS added to make sure we don't end up in a timing race and infinite loop sending Acks
    unsigned short sender = SENDERID;
    short _RSSI = RSSI; // save payload received RSSI value
    writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
    unsigned int now = millis();
    while (!canSend() && millis() - now < RF69_CSMA_LIMIT_MS) receiveDone();
    SENDERID = sender;    // TWS: Restore SenderID after it gets wiped out by receiveDone()
    sendFrame(sender, buffer, bufferSize, false, true);
    RSSI = _RSSI; // restore payload RSSI
}
int RFM69::getFrequency()
{
    return RF69_FSTEP * (((unsigned int) readReg(REG_FRFMSB) << 16) + ((unsigned short) readReg(REG_FRFMID) << 8) + readReg(REG_FRFLSB));
}
void RFM69::setFrequency(unsigned int freqHz)
{
    unsigned char oldMode = _mode;
    if (oldMode == RF69_MODE_TX)
    {
        setMode(RF69_MODE_RX);
    }
    freqHz /= RF69_FSTEP; // divide down by FSTEP to get FRF
    writeReg(REG_FRFMSB, freqHz >> 16);
    writeReg(REG_FRFMID, freqHz >> 8);
    writeReg(REG_FRFLSB, freqHz);
    if (oldMode == RF69_MODE_RX)
    {
        setMode(RF69_MODE_SYNTH);
    }
    setMode(oldMode);
}
void RFM69::encrypt(unsigned char* key)
{
    setMode(RF69_MODE_STANDBY);
    if (key != 0)
    {
        unsigned char data = REG_AESKEY1 | 0x80;
        wiringPiSPIDataRW(0, &data, 1);
        for (unsigned char i = 0; i < 16; i++)
            wiringPiSPIDataRW(0, &key[i], 1);
    }
    writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFE) | (key ? 1 : 0));
}
short RFM69::readRSSI(bool forceTrigger) // *current* signal strength indicator; e.g. < -90dBm says the frequency channel is free + ready to transmit
{
    short rssi = 0;
    if (forceTrigger)
    {
        // RSSI trigger not needed if DAGC is in continuous mode
        writeReg(REG_RSSICONFIG, RF_RSSI_START);
        while ((readReg(REG_RSSICONFIG) & RF_RSSI_DONE) == 0x00); // wait for RSSI_Ready
    }
    rssi = -readReg(REG_RSSIVALUE);
    rssi >>= 1;
    return rssi;
}
void RFM69::spyMode(bool onOff)
{
    _spyMode = onOff;
}
void RFM69::setHighPower(bool onOFF) // has to be called after initialize() for RFM69HW
{
    _isRFM69HW = onOFF;
    writeReg(REG_OCP, _isRFM69HW ? RF_OCP_OFF : RF_OCP_ON);
    if (_isRFM69HW) // turning ON
        writeReg(REG_PALEVEL, (readReg(REG_PALEVEL) & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON); // enable P1 & P2 amplifier stages
    else
        writeReg(REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | _powerLevel); // enable P0 only
}
void RFM69::setPowerLevel(unsigned char level) // reduce/increase transmit power level
{
    _powerLevel = (level > 31 ? 31 : level);
    if (_isRFM69HW) _powerLevel /= 2;
    writeReg(REG_PALEVEL, (readReg(REG_PALEVEL) & 0xE0) | _powerLevel);
}
void RFM69::RFM69_sleep()
{
    setMode(RF69_MODE_SLEEP);
}
unsigned char RFM69::readTemperature(unsigned char calFactor) // get CMOS temperature (8bit)
{
    setMode(RF69_MODE_STANDBY);
    writeReg(REG_TEMP1, RF_TEMP1_MEAS_START);
    while ((readReg(REG_TEMP1) & RF_TEMP1_MEAS_RUNNING));
    return ~readReg(REG_TEMP2) + COURSE_TEMP_COEF + calFactor;
}
void RFM69::rcCalibration()
{
    writeReg(REG_OSC1, RF_OSC1_RCCAL_START);
    while ((readReg(REG_OSC1) & RF_OSC1_RCCAL_DONE) == 0x00);
}
void RFM69::sendFrame(unsigned short toAddress, const void* buffer, unsigned char bufferSize, bool requestACK, bool sendACK)
{
    setMode(RF69_MODE_STANDBY); // turn off receiver to prevent reception while filling fifo
    while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00); // wait for ModeReady
    if (bufferSize > RF69_MAX_DATA_LEN) bufferSize = RF69_MAX_DATA_LEN;

    // control byte
    unsigned char CTLbyte = 0x00;
    if (sendACK)
        CTLbyte = RFM69_CTL_SENDACK;
    else if (requestACK)
        CTLbyte = RFM69_CTL_REQACK;

    if (toAddress > 0xFF) CTLbyte |= (toAddress & 0x300) >> 6; //assign last 2 bits of address if > 255
    if (_address > 0xFF) CTLbyte |= (_address & 0x300) >> 8;   //assign last 2 bits of address if > 255

    // write to FIFO
    unsigned char data[bufferSize+5];
    data[0] = REG_FIFO | 0x80;
    data[1] = bufferSize + 3;
    data[2] = (unsigned char)toAddress;
    data[3] = (unsigned char)_address;
    data[4] = CTLbyte;
    unsigned char i;
    for (i = 0; i < bufferSize; i++)
    {
        data[5+i] = ((unsigned char*) buffer)[i];
    }
    wiringPiSPIDataRW(0, data, bufferSize+5);
    readReg(REG_IRQFLAGS2);
    // no need to wait for transmit mode to be ready since its handled by the radio
    setMode(RF69_MODE_TX);
    while ((readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT) == 0x00) delay(1); // wait for PacketSent
    _haveData = false;
}
void RFM69::receiveBegin()
{
    DATALEN = 0;
    SENDERID = 0;
    TARGETID = 0;
    PAYLOADLEN = 0;
    ACK_REQUESTED = 0;
    ACK_RECEIVED = 0;
    RSSI = 0;
    if (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY)
        writeReg(REG_PACKETCONFIG2, (readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART); // avoid RX deadlocks
    writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01); // set DIO0 to "PAYLOADREADY" in receive mode
    setMode(RF69_MODE_RX);
}
void RFM69::setMode(unsigned char mode)
{
    if (mode == _mode)
        return;

    switch (mode) {
        case RF69_MODE_TX:
            writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER);
            if (_isRFM69HW) setHighPowerRegs(true);
                break;
        case RF69_MODE_RX:
            writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER);
            if (_isRFM69HW) setHighPowerRegs(false);
                break;
        case RF69_MODE_SYNTH:
            writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER);
            break;
        case RF69_MODE_STANDBY:
            writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY);
            break;
        case RF69_MODE_SLEEP:
            writeReg(REG_OPMODE, (readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP);
            break;
        default:
            return;
    }
    _mode = mode;
}
void RFM69::setHighPowerRegs(bool onOff)
{
    writeReg(REG_TESTPA1, onOff ? 0x5D : 0x55);
    writeReg(REG_TESTPA2, onOff ? 0x7C : 0x70);
}
void RFM69::interruptHandler()
{
    if (_mode == RF69_MODE_RX && (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY))
    {
        setMode(RF69_MODE_STANDBY);
        PAYLOADLEN =readReg(REG_FIFO & 0x7F);
        PAYLOADLEN = PAYLOADLEN > 66 ? 66 : PAYLOADLEN; // precaution
        unsigned char data[PAYLOADLEN+1];
        data[0]=REG_FIFO & 0x7F;
        wiringPiSPIDataRW(0, data, PAYLOADLEN+1);
        TARGETID = data[1];
        SENDERID = data[2];
        unsigned char CTLbyte = data[3];
        TARGETID |= ((unsigned short)(CTLbyte) & 0x0C) << 6; //10 bit address (most significant 2 bits stored in bits(2,3) of CTL byte
        SENDERID |= ((unsigned short)(CTLbyte) & 0x03) << 8; //10 bit address (most sifnigicant 2 bits stored in bits(0,1) of CTL byte
        if(!(_spyMode || TARGETID == _address || TARGETID == RF69_BROADCAST_ADDR) || PAYLOADLEN < 3) // address situation could receive packets that are malformed and don't fit this libraries extra fields
        {
            PAYLOADLEN = 0;
            receiveBegin();
            _haveData = false;
            return;
        }

        DATALEN = PAYLOADLEN - 3;
        ACK_RECEIVED = CTLbyte & RFM69_CTL_SENDACK; // extract ACK-received flag
        ACK_REQUESTED = CTLbyte & RFM69_CTL_REQACK; // extract ACK-requested flag

        unsigned char i;
        for (i = 0; i < DATALEN; i++) DATA[i] = data[i+3];

        DATA[DATALEN] = 0; // add null at end of string
        setMode(RF69_MODE_RX);
    }
    RSSI = readRSSI(0);
}
unsigned char RFM69::readReg(unsigned char addr)
{
    unsigned char data[2]={0};
    data[0]=addr&0x7F;
    wiringPiSPIDataRW(0, data, 2);
    //cout << "Read Register: " << (int) data[1] << endl;
    return data[1];
}
void RFM69::writeReg(unsigned char addr, unsigned char val)
{
    unsigned char data[2]={0};
    data[0]=addr|0x80;
    data[1]=val;
    //cout << "Write Register: " << (int) data[1] << endl;
    wiringPiSPIDataRW(0, data, 2);
}
void RFM69::isr0()
{
    if(_mode == RF69_MODE_RX)
        _haveData = true;
}

} /* namespace cubesat */
