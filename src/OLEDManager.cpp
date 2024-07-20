// OLEDManager.cpp
#include "OLEDManager.h"

OLEDManager::OLEDManager(uint8_t w, uint8_t h, int8_t rst_pin)
    : display(w, h, &Wire, rst_pin), width(w), height(h), eventDisplayStartTime(0), isDisplayingEvent(false) {}

bool OLEDManager::begin(uint8_t i2c_address) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2c_address)) {
        return false;
    }
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    return true;
}

void OLEDManager::displayTime(const DateTime& now) {
    if (isDisplayingEvent && millis() - eventDisplayStartTime < 5000) {
        return; // Continue displaying the event for 5 seconds
    }

    isDisplayingEvent = false;
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.printf("%04d-%02d-%02d", now.year(), now.month(), now.day());
    
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.printf("%02d:%02d:%02d", now.hour(), now.minute(), now.second());
    
    display.display();
}

void OLEDManager::displayEvent(const EventManager::Event& event) {
    display.clearDisplay();
    display.setTextSize(1);
    
    display.setCursor(0, 0);
    display.println("Event Triggered!");
    
    display.setCursor(0, 16);
    display.printf("Scenario: %d", event.scenario);
    
    display.setCursor(0, 26);
    display.printf("Cycle: %d", event.cycle);
    
    display.setCursor(0, 36);
    display.println(event.description);
    
    display.display();
    
    isDisplayingEvent = true;
    eventDisplayStartTime = millis();
}

void OLEDManager::clear() {
    display.clearDisplay();
    display.display();
}