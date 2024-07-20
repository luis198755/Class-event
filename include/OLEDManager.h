// OLEDManager.h
#ifndef OLED_MANAGER_H
#define OLED_MANAGER_H

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "RTClib.h"

class OLEDManager {
public:
    OLEDManager(uint8_t w = 128, uint8_t h = 64, int8_t rst_pin = -1);
    bool begin(uint8_t i2c_address = 0x3C);
    void displayTime(const DateTime& now);
    void clear();

private:
    Adafruit_SSD1306 display;
    uint8_t width;
    uint8_t height;
};

#endif 