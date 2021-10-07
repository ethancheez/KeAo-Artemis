/** @mainpage
    This library implements packet data transmittion via RFM69CW radiomodules.

    In order to use this library you need to include the @ref rfm69.h file, 
    declare extern global variables and call @ref rfm69_init() function, tat's
    all.
 
    After init radiomodule will switch into receive mode and will wait the 
    packet by the radio channel. To transmit the packet you need to place your
    data into @ref packet_buffer and call rfm69_transmit_start() function, then 
    after transmitting data, radiomodule will be switched by interrupt into 
    receive mode again.

    If radiomodule received the package successfully, the value of @ref 
    rfm69_condition equals to @ref RFM69_NEW_PACK and received data are 
    alreary in the @ref packet_buffer array. Then, when you are ready to 
    receive new data, you need to start the receive mode again by calling 
    rfm69_receive_start() function. 

    Note that radiomodule settings are in the rfm69.h file, packet size is in
    @ref packet_size global variable and the packet data are in @ref 
    packet_buffer global array and radiomodule state is in @ref rfm69_condition 
    variable.
*/
#include <iostream>
#include <unistd.h>
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
RFM69HCW::RFM69HCW(uint8_t address, uint8_t device, SPIDevice::SPIMODE spi_mode){
    /* call rfm69_init() */
    rfm69_mcu_init(address, device, spi_mode);
    rfm69_init();
}

/** 
    @function
    External interrupt handler. It is called after radiomodule has transmitted
    or received the packet.
*/
void RFM69HCW::EXTI2_IRQHandler(void)
{
    int tmp;

    switch(rfm69_condition)
    {
        case RFM69_SLEEP :
            rfm69_sleep();
            break;
        case RFM69_STBY :
            rfm69_stby();
            break;
        case RFM69_RX :
            if(rfm69_read(REGIRQFLAGS2) & (1<<PAYLOADREADY))
            {
                tmp = rfm69_receive_small_packet();
                rfm69_stby();

                if(tmp) rfm69_condition = RFM69_NEW_PACK;
                else    rfm69_receive_start();
            }
            break;
        case RFM69_TX :
            if(rfm69_read(REGIRQFLAGS2) & (1<<PACKETSENT))
            {
                rfm69_receive_start();
            }
            break;
    }
    //EXTI_ClearITPendingBit(CRCOK_PKSent_Line);
}

/** 
    @function
    External interrupt handler. It is called after radiomodule FIFO threshold 
    level has been exceeded. This interrupt is useful for transmitting and 
    receiving packets bigger than 64 bytes. And it is not used in this firmware.
*/
void RFM69HCW::EXTI1_IRQHandler(void)
{
    switch(rfm69_condition)
    {
        case RFM69_SLEEP :
            break;
        case RFM69_STBY :
            break;
        case RFM69_RX :
            break;
        case RFM69_TX :
            break;
    }
    //EXTI_ClearITPendingBit(FifoLevel_Line);
}

/** 
    @function
    External interrupt handler. It is called after rerceived signal has 
    exceeded FIFO threshold. And it is not used at that moment.
*/
void RFM69HCW::EXTI0_IRQHandler(void)
{
    switch(rfm69_condition)
    {
        case RFM69_SLEEP :
            rfm69_sleep();
            break;
        case RFM69_STBY :
            rfm69_stby();
            break;
        case RFM69_RX :
            break;
        case RFM69_TX :
            break;
    }
    //EXTI_ClearITPendingBit(SyncAddr_Line);
}

/**
    @function
    Accessor method for packet_buffer
    @param data - array to copy packet data into from packet_buffer
*/
void RFM69HCW::getData( char *data){
    int i;
    for(i = 0; i < RFM69_BUFFER_SIZE; i++ ){
        data[i] = this->packet_buffer[i];
    }
}


/**
 * @function setMode
 *
 * @param byte newMode - Could use RF69_MODE_TX, RF69_MODE_RX, RF69_MODE_SYNTH, 
 * RF69_MODE_STANDBY or RF69_MODE_SLEEP
 */
void RFM69HCW::setMode(uint8_t mode) {

  switch (mode) {
    case TX_MODE:
      rfm69_write(REGOPMODE, (rfm69_read(REGOPMODE) & 0xE3) | TX_MODE);
      break;
    case RX_MODE:
      rfm69_write(REGOPMODE, (rfm69_read(REGOPMODE) & 0xE3) | RX_MODE);
      break;
    case FS_MODE:
      rfm69_write(REGOPMODE, (rfm69_read(REGOPMODE) & 0xE3) | FS_MODE);
      break;
    case STBY_MODE:
      rfm69_write(REGOPMODE, (rfm69_read(REGOPMODE) & 0xE3) | STBY_MODE);
      break;
    case SLEEP_MODE:
      rfm69_write(REGOPMODE, (rfm69_read(REGOPMODE) & 0xE3) | SLEEP_MODE);
      break;
    default: return;
    }
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
    if( this->spi->writeRegister(address | 0x80, data) < 0 ){
        cerr<<"RFM69HCW::rfm69_write->Error writing to register "<<address<<endl;
    }
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

    data =  this->spi->readRegister( address & 0x7f );
	//cout << "The value that was received is: " << data << endl;
	return data;
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
int RFM69HCW::rfm69_init(){
    int j;

    //this->spi->debugDumpRegisters();

    // Reset radio
    printf("Resetting Radio . . .\n");
    GPIO *outGPIO = new GPIO(80);
    outGPIO->setDirection(OUTPUT);
    outGPIO->setValue(HIGH);
    COSMOS_SLEEP(0.1); // wait 0.1s
    outGPIO->setValue(LOW);
    COSMOS_SLEEP(0.1); // wait 0.1s
    delete outGPIO;

    // Ensure that the radio is able to read and write through SPI
    for(int cycle = 0; rfm69_read(REGSYNCVALUE1) != 0xaa; cycle++) {
        if(cycle > 100) {
            printf("SPI was not properly initialized. Exiting . . .\n");
            exit (0);
        }
        rfm69_write(REGSYNCVALUE1, 0xaa);
    }

    for(int cycle = 0; rfm69_read(REGSYNCVALUE2) != 0x55; cycle++) {
        if(cycle > 100) {
            printf("SPI was not properly initialized. Exiting . . .\n");
            exit (0);
        }
        rfm69_write(REGSYNCVALUE2, 0x55);
    }

//  RFM69 initialization
    rfm69_write(REGOPMODE, SEQUENCEROFF | LISTENOFF | STBY_MODE);
    rfm69_write(REGDATAMODUL, PACKET_MODE | FSK | NO_SHAPING);

    rfm69_write(REGFDEVMSB, DEVMSB_5000);
    rfm69_write(REGFDEVLSB, DEVLSB_5000);

    rfm69_write(REGBITRATEMSB, REGBITRATEMSB_DEF);
    rfm69_write(REGBITRATELSB, REGBITRATELSB_DEF);

    // FREQUENCY BAND
    rfm69_write(REGFRFMSB, FRFMSB_915);
    rfm69_write(REGFRFMID, FRFMID_915);
    rfm69_write(REGFRFLSB, FRFLSB_915);

    rfm69_write(REGRXBW, RXBW_DCCFREQ_010 | RXBW_MANT_16 | RXBW_EXP_2);

    rfm69_write(REGAFCCTRL, REGAFCCTRL_DEF);

//  rfm69_write(REGLISTEN1, REGLISTEN1_DEF);
//  rfm69_write(REGLISTEN2, REGLISTEN2_DEF);
//  rfm69_write(REGLISTEN3, REGLISTEN3_DEF);

//    rfm69_write(REGPALEVEL, REGPALEVEL_DEF);
//    rfm69_write(REGPARAMP, REGPARAMP_DEF);
//    rfm69_write(REGOCP, REGOCP_DEF);
//    rfm69_write(REGLNA, REGLNA_DEF);


//  rfm69_write(REGOOKPEAK, REGOOKPEAK_DEF);
//  rfm69_write(REGOOKAVG, REGOOKAVG_DEF);
//  rfm69_write(REGOOKFIX, REGOOKFIX_DEF);

//    rfm69_write(REGAFCFEI, REGAFCFEI_DEF);

    rfm69_write(REGDIOMAPPING1, REGDIOMAPPING1_DEF);
 //   rfm69_write(REGDIOMAPPING2, REGDIOMAPPING2_DEF);

    rfm69_write(REGRSSITHRESH, 220);
    rfm69_write(REGFIFOTHRES, FIFOTHRESH_TXSTART_FIFONOTEMPTY | FIFOTHRESH_VALUE);

    //rfm69_write(REGPREAMBLEMSB, REGPREAMBLEMSB_DEF);
    //rfm69_write(REGPREAMBLELSB, REGPREAMBLELSB_DEF);

    rfm69_write(REGSYNCCONFIG, SYNC_ON | SYNC_FIFOFILL_AUTO | SYNC_SIZE_2 | SYNC_TOL_0 );
    rfm69_write(REGSYNCVALUE1, REGSYNCVALUE1_DEF);
    rfm69_write(REGSYNCVALUE2, REGSYNCVALUE2_DEF);// set network id

    rfm69_write(REGPACKETCONFIG1, REGPACKETCONFIG1_DEF);
    rfm69_write(REGPAYLOADLENGTH, REGPAYLOADLENGTH_DEF);
    rfm69_write(REGNODEADRS, REGNODEADRS_DEF);
    rfm69_write(REGBROADCASTADRS, REGBROADCASTADRS_DEF);
    rfm69_write(REGAUTOMODES, REGAUTOMODES_DEF);

    rfm69_write(REGPACKETCONFIG2, PACKET2_RXRESTARTDELAY_2BITS | PACKET2_AUTORXRESTART_ON | PACKET2_AES_OFF);

//  rfm69_write(REGAESKEY1, REGAESKEY1_DEF);
//  rfm69_write(REGAESKEY2, REGAESKEY2_DEF);
//  rfm69_write(REGAESKEY3, REGAESKEY3_DEF);
//  rfm69_write(REGAESKEY4, REGAESKEY4_DEF);
//  rfm69_write(REGAESKEY5, REGAESKEY5_DEF);
//  rfm69_write(REGAESKEY6, REGAESKEY6_DEF);
//  rfm69_write(REGAESKEY7, REGAESKEY7_DEF);
//  rfm69_write(REGAESKEY8, REGAESKEY8_DEF);
//  rfm69_write(REGAESKEY9, REGAESKEY9_DEF);
//  rfm69_write(REGAESKEY10, REGAESKEY10_DEF);
//  rfm69_write(REGAESKEY11, REGAESKEY11_DEF);
//  rfm69_write(REGAESKEY12, REGAESKEY12_DEF);
//  rfm69_write(REGAESKEY13, REGAESKEY13_DEF);
//  rfm69_write(REGAESKEY14, REGAESKEY14_DEF);
//  rfm69_write(REGAESKEY15, REGAESKEY15_DEF);
//  rfm69_write(REGAESKEY16, REGAESKEY16_DEF);

    rfm69_write(REGTESTDAGC, DAGC_IMPROVED_LOWBETA0);

    rfm69_write(REGIRQFLAGS2, 1<<FIFOOVERRUN);
    this->spi->debugDumpRegisters();

    rfm69_setHighPower(1);

    /* TODO: remove this busy waiting. Replace with call to sleep */
    for (j=0 ; j<99999 ; ++j);
    rfm69_receive_start();
    // SPI check
    int ret = rfm69_read(REGRXBW);
    if ( ret != (RXBW_DCCFREQ_010 | RXBW_MANT_16 | RXBW_EXP_2) ) {
        rfm69_condition = RFM69_SPI_FAILED;
        cerr<<"RFM69HCW::rfm69_init->SPI failed. ret: '"<<ret<<"'"<<endl;
        return -1;
    }else{
        cout << endl << "-------------------------------------------" << endl;
        cout << "RADIO SUCCESSFULLY INITIALIZED" << endl;
        cout << "-------------------------------------------" << endl;
        return 0;
    }
}

int RFM69HCW::getRSSI(int forceTrigger){
  int rssi = 0;
  if (forceTrigger == 1) {
    //RSSI trigger not needed if DAGC is in continuous mode
    rfm69_write(REGRSSICONFIG, RSSISTART);
    while ((rfm69_read(REGRSSICONFIG) & RSSIDONE) == 0x00); // Wait for RSSI_Ready
  }
  rssi = -rfm69_read(REGRSSIVALUE);
  rssi >>= 1;
  rssi += 20;
  return rssi;

}

/** 
    @function
    Start packet transmission. This function assumes that radiomodule is in receive, sleep or standby mode. Also do not forget to 
    switch the transmitter off when package is already transmitted (interrupts will do that).
    @param packet_size_loc - size of the packet
    @param address - destination address (use broadcast address 0xff if you do not know any)
    @return negative value if something wrong (package is too big, or SPI doesn't work), and return 0 if all is well
*/
int RFM69HCW::rfm69_transmit_start(uint8_t packet_size_loc, uint8_t address)
{
    int i;

    packet_size = packet_size_loc;
    if(packet_size > RFM69_BUFFER_SIZE) return -1;                          // check size of the package
    
    switch(rfm69_condition)
    {
        case RFM69_SPI_FAILED :
            return -1;
        case RFM69_SLEEP :
            rfm69_clear_fifo();
            break;
        case RFM69_STBY :
            rfm69_clear_fifo();
            break;
        case RFM69_RX :
            break;
        case RFM69_TX :
            return -1;
    }
    
    rfm69_condition = RFM69_TX;
    rfm69_write(REGOPMODE, REGOPMODE_DEF | TX_MODE);                        // makes transmitter on

    rfm69_write(REGFIFO, packet_size+1);                                    // transmit the packet size into FIFO
    rfm69_write(REGFIFO, address);                                          // transmit address into FIFO
    for(i=0 ; i<packet_size ; ++i)  rfm69_write(REGFIFO, packet_buffer[i]); // transmit packet payload into FIFO

    return 0;
}

/** 
    @function
    Switch radiomodule into receiver mode.
*/
void RFM69HCW::rfm69_receive_start(void)
{
    rfm69_condition = RFM69_RX;
    rfm69_write(REGOPMODE, REGOPMODE_DEF | RX_MODE);
}

/** 
    @function
    This function reads received packet into the @ref packet_buffer
    @return size of the received packet, and if size of the received package exceeded the buffer size, return -1
*/
int RFM69HCW::rfm69_receive_small_packet(void)
{
    unsigned int timeout = 10000;
    /* receive data until timeout or valid data */
    while(1){
        if (rfm69_read(REGIRQFLAGS2) & PAYLOADREADY){
            rfm69_write(REGPACKETCONFIG2, (rfm69_read(REGPACKETCONFIG2) &
                0xFB) | RESTARTRX); // avoid RX deadlocks
        }
        setMode(RX_MODE);

        // Receive Data until timeout (aprox 2s)
        while((rfm69_read(REGIRQFLAGS2) & PAYLOADREADY) == 0) {
            timeout--;
            if(timeout == 0) {
                setMode(STBY_MODE);
                return -2;
            }
        }

        packet_size = rfm69_read(REGFIFO);   // read the packet size
        if(packet_size > 63) return -1;    // check size of the package

        rfm69_read(REGFIFO);    // drop address
        --packet_size;

        int i;
        // read package from FIFO
        for(i=0 ; i<packet_size ; ++i) packet_buffer[i] = rfm69_read(REGFIFO);  
        rfm69_clear_fifo();

        return packet_size;
    }
}

void RFM69HCW::rfm69_setHighPower(int onOff) {
    if(onOff) {
        rfm69_write(REGOCP, RF_OCP_OFF);
        rfm69_write(REGPALEVEL, (rfm69_read(REGPALEVEL) & 0x1f) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON);
    }
    else {
        rfm69_write(REGOCP, RF_OCP_ON);
        rfm69_write(REGPALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | MAX_POWER_LEVEL);
    }
}

/**
    @function
    Switch radiomodule into sleep mode.
*/
void RFM69HCW::rfm69_sleep(void)
{
    rfm69_write(REGOPMODE, REGOPMODE_DEF | SLEEP_MODE);
    rfm69_condition = RFM69_SLEEP;
}

/**
    @function
    Switch radiomodule into standby mode.
*/
void RFM69HCW::rfm69_stby(void)
{
    rfm69_write(REGOPMODE, REGOPMODE_DEF | STBY_MODE);
    rfm69_condition = RFM69_STBY;
}

/**
    @function
    Clear FIFO of radiomodule.
*/
void RFM69HCW::rfm69_clear_fifo(void)
{
    int i;
    for(i=0 ; i<RFM69_BUFFER_SIZE ; ++i)   rfm69_read(REGFIFO);     // read every register one by one
    rfm69_write(REGIRQFLAGS2, 1<<FIFOOVERRUN);                      // clear flag if overrun
}

/**
    @function
    Destructor
*/
RFM69HCW::~RFM69HCW(){
    this->spi->close();
    delete this->spi;
}

} /* cubesat namespace */

