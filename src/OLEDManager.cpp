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
    // Initial flicker
    flickerEffect();
    
    // Scanning lines
    scanningLines();
    
    // Geometric shapes
    //geometricShapes();
    
    // Text emergence
    textEmergence();
    
    // Show dynamic noise for 2 seconds
    unsigned long startTime = millis();
    while (millis() - startTime < 2000) {
        drawTVNoise();
        delay(50);
    }

    // Apply fade-out effect
    fadeOutEffect();
}

void OLEDManager::flickerEffect() {
    clear();
    for (int i = 0; i < 10; i++) {
        display.invertDisplay(true);
        delay(50);
        display.invertDisplay(false);
        delay(30);
    }
}

void OLEDManager::scanningLines() {
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < height; y += 4) {
            display.clearDisplay();
            display.drawFastHLine(0, y, width, SSD1306_WHITE);
            display.display();
            delay(10);
        }
    }
}

void OLEDManager::geometricShapes() {
    for (int i = 0; i < 30; i++) {
        display.clearDisplay();
        int x = random(width);
        int y = random(height);
        int size = random(10, 30);
        
        switch (random(3)) {
            case 0:
                display.drawCircle(x, y, size/2, SSD1306_WHITE);
                break;
            case 1:
                display.drawRect(x, y, size, size, SSD1306_WHITE);
                break;
            case 2:
                display.drawTriangle(x, y, x + size, y, x + size/2, y - size, SSD1306_WHITE);
                break;
        }
        display.display();
        delay(100);
    }
}

void OLEDManager::textEmergence() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    
    const char* bootText = "BOOTING...";
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(bootText, 0, 0, &x1, &y1, &w, &h);
    int x = (width - w) / 2;
    int y = (height - h) / 2;
    
    for (int i = 0; bootText[i] != '\0'; i++) {
        display.setCursor(x, y);
        for (int j = 0; j <= i; j++) {
            display.print(bootText[j]);
        }
        display.display();
        delay(200);
    }
    delay(1000);
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