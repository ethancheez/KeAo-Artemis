#include "artemis_channels.h"

void Artemis::Teensy::Channels::astrodev_channel()
{
    Artemis::Teensy::Radio::Astrodev astrodev;

    Serial.println(astrodev.Init());

    std::vector<uint8_t> data = {'H', 'e', 'l', 'l', 'o'};
    while (true)
    {
        delay(3000);
        astrodev.SendData(data);
    }
}
