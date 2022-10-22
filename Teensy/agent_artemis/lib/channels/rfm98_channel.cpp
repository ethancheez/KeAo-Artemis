#include <artemis_channels.h>

void Artemis::Teensy::Channels::rfm98_channel()
{
    Artemis::Teensy::Radio::RFM98 rfm98(RFM98_CS_PIN, RFM98_INT_PIN, hardware_spi1);
    
    rfm98.RFM98_INIT();
    delay(1000);

    while (true)
    {
        rfm98.RFM98_SEND("hello world");
        delay(5000);
    }
}