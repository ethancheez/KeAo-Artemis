#include "artemis_channels.h"

Artemis::Teensy::Radio::RFM98 rfm98(RFM98_CS_PIN, RFM98_INT_PIN, hardware_spi1);

void Artemis::Teensy::Channels::rfm98_channel() {
    rfm98.RFM98_INIT();

    while(true) {
        Serial.println("hi");
        delay(1000);
    }
}