#include <Arduino.h>
// Example usage in main Arduino sketch
#include <Wire.h>
#include "RTClib.h"
#include "EventManager.h"
#include "OLEDManager.h"

RTC_DS3231 rtc;
EventManager eventManager(rtc);
OLEDManager oledManager;

void onEventTriggered(const EventManager::Event& event) {
    Serial.printf("Event triggered - Scenario: %d, Cycle: %d, Desc: %s\n",
    event.scenario, event.cycle, event.description.c_str());
    // Implement your traffic light control logic here
    oledManager.displayEvent(event);
} 

void setup() {
    Serial.begin(115200);
    Wire.begin();  // Start I2C communication

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    if (!oledManager.begin()) {
        Serial.println("SSD1306 allocation failed");
        while (1);
    }

    eventManager.begin();
    eventManager.setEventCallback(onEventTriggered);
    
    // Add your events
    eventManager.addEvent(2023, 7, 20, 8, 0, 0, 1, 1, "Morning Rush Hour");
    eventManager.addEvent(2024, 7, 19, 22, 32, 0, 2, 1, "Evening Rush Hour");

    eventManager.printEvents();
}

void loop() {
    static unsigned long lastDisplayUpdate = 0;
    unsigned long currentMillis = millis();

    eventManager.update();

    // Update OLED display every second
    if (currentMillis - lastDisplayUpdate >= 1000) {
        DateTime now = rtc.now();
        oledManager.displayTime(now);
        lastDisplayUpdate = currentMillis;
    }

    // Other loop code...
}