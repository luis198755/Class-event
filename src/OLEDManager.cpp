// OLEDManager.cpp
#include "OLEDManager.h"

OLEDManager::OLEDManager(uint8_t w, uint8_t h, int8_t rst_pin)
    : display(w, h, &Wire, rst_pin), width(w), height(h) {}

bool OLEDManager::begin(uint8_t i2c_address) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2c_address)) {
        return false;
    }
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    return true;
}

void OLEDManager::displayTime(const DateTime& now) {
    display.clearDisplay();
    display.setCursor(0, 0);
    
    // Display date
    display.setTextSize(1);
    display.printf("%04d-%02d-%02d", now.year(), now.month(), now.day());
    
    // Display time
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    
    display.display();
}

void OLEDManager::clear() {
    display.clearDisplay();
    display.display();
}