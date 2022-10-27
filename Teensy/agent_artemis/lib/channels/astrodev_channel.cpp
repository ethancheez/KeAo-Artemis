#include "artemis_channels.h"
#include <Arduino.h>

using namespace Artemis::Teensy::Radio;

void Artemis::Teensy::Channels::astrodev_channel()
{
    int32_t iretn;
    Astrodev astrodev;

    Serial.println("Initializing...");
    Serial.println(astrodev.Init(&Serial8));

    if ((iretn = astrodev.Ping()) < 0)
    {
        Serial.println("Failed to ping astrodev");
        exit(-1);
    }

    astrodev.Reset();
    delay(20000);

    struct Astrodev::tcv_config astrodev_tcv_config;
    astrodev_tcv_config.interface_baud_rate = 0;
    astrodev_tcv_config.power_amp_level = 255;
    astrodev_tcv_config.rx_baud_rate = 1;
    astrodev_tcv_config.tx_baud_rate = 1;
    astrodev_tcv_config.ax25_preamble_length = 20;
    astrodev_tcv_config.ax25_postamble_length = 20;
    // astrodev_tcv_config.config1;
    // astrodev_tcv_config.config2;
    astrodev_tcv_config.rx_modulation = (uint8_t)Astrodev::Modulation::ASTRODEV_MODULATION_GFSK;
    astrodev_tcv_config.tx_modulation = (uint8_t)Astrodev::Modulation::ASTRODEV_MODULATION_GFSK;
    astrodev_tcv_config.tx_freq_high = 450000 / 65536;
    astrodev_tcv_config.tx_freq_low = 450000 % 65536;
    astrodev_tcv_config.rx_freq_high = 450000 / 65536;
    astrodev_tcv_config.rx_freq_low = 450000 % 65536;
    memcpy(astrodev_tcv_config.ax25_source, "SOURCE", 6);
    memcpy(astrodev_tcv_config.ax25_destination, "DESTIN", 6);

    while ((iretn = astrodev.SetTCVConfig(astrodev_tcv_config)) < 0)
    {
        Serial.println("Resetting");
        astrodev.Reset();
        Serial.println("Failed to settcvconfig astrodev");
        delay(20000);
    }
    if ((iretn = astrodev.GetTCVConfig()) < 0)
    {
        Serial.println("Failed to gettcvconfig astrodev");
    }

    std::vector<uint8_t> data = {'H', 'e', 'l', 'l', 'o'};
    while (true)
    {
        if ((iretn = astrodev.GetTelemetry()) < 0)
        {
            Serial.println("Failed to telemetry astrodev");
            delay(1000);
        }

        delay(1000);
        astrodev.SendData(data);
    }
}
