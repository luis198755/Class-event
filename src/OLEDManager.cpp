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

void OLEDManager::showTVTurnOnEffect() {
    // Simulate TV turn on effect
    for (int i = 0; i < 3; i++) {
        drawTVNoise();
        delay(100);
        display.clearDisplay();
        display.display();
        delay(50);
    }
    
    // Simulate horizontal line sweep
    for (int y = 0; y < height; y += 2) {
        display.drawFastHLine(0, y, width, SSD1306_WHITE);
        display.display();
        delay(10);
    }
    
    delay(200);
    display.clearDisplay();
    display.display();
    
    // Show dynamic noise for 3 seconds
    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
        drawTVNoise();
        delay(50);  // Adjust this value to control the speed of noise changes
    }

    // Apply fade-out effect
    fadeOutEffect();
}

void OLEDManager::drawTVNoise() {
    display.clearDisplay();  // Clear before drawing new noise
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (random(2) == 1) {
                display.drawPixel(x, y, SSD1306_WHITE);
            }
        }
    }
    display.display();
}

void OLEDManager::fadeOutEffect() {
    uint8_t* buffer = display.getBuffer();
    int bufferSize = width * height / 8;
    
    for (int step = 0; step < 16; step++) {
        for (int i = 0; i < bufferSize; i++) {
            buffer[i] = buffer[i] & random(0xFF);  // Randomly clear bits
        }
        display.display();
        delay(50);  // Adjust for faster/slower fade
    }
    
    // Ensure the display is completely clear at the end
    display.clearDisplay();
    display.display();
}