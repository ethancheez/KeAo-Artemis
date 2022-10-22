#include "artemis_channels.h"

void Artemis::Teensy::Channels::rfm23_channel()
{
    Artemis::Teensy::Radio::RFM23 rfm23(RFM23_CS_PIN, RFM23_INT_PIN, hardware_spi1);
    
    rfm23.RFM23_INIT();

    while (true)
    {
        // rfm23.RFM23_RECV();
        rfm23.RFM23_SEND("hi");
        delay(5000);
    }
}