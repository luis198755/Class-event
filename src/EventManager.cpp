// EventManager.cpp
#include "EventManager.h"

EventManager::EventManager(RTC_DS3231& rtc)
    : rtc(rtc), currentScenario(0), currentCycle(0), isProcessingEvents(false), lastProcessTime(0) {}

void EventManager::begin() {
    // No need for timer setup, we'll use millis() for timing
}

bool EventManager::addEvent(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, 
                            uint8_t scenario, uint8_t cycle, const String& description) {
    if (!isValidDate(year, month, day) || hour >= 24 || minute >= 60 || second >= 60) {
        Serial.println(F("Invalid date or time"));
        return false;
    }
    events.emplace(year, month, day, hour, minute, second, scenario, cycle, description);
    return true;
}

bool EventManager::removeEvent(uint64_t timeCode) {
    for (auto it = events.begin(); it != events.end(); ++it) {
        if (it->timeCode == timeCode) {
            events.erase(it);
            return true;
        }
    }
    return false;
}

void EventManager::clearEvents() {
    events.clear();
    while (!pendingEvents.empty()) {
        pendingEvents.pop();
    }
}

void EventManager::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastProcessTime >= 1000) {  // Check every second
        lastProcessTime = currentTime;
        
        DateTime now = rtc.now();
        uint64_t currentTimeCode = Event::encodeTime(now.year(), now.month(), now.day(), 
                                                     now.hour(), now.minute(), now.second());
        
        auto nextEvent = findNextEvent(currentTimeCode);
        
        while (nextEvent != events.end() && currentTimeCode >= nextEvent->timeCode) {
            pendingEvents.push(*nextEvent);
            events.erase(nextEvent);
            nextEvent = findNextEvent(currentTimeCode);
        }

        processNextPendingEvent();
    }
}

uint8_t EventManager::getCurrentScenario() const {
    return currentScenario;
}

uint8_t EventManager::getCurrentCycle() const {
    return currentCycle;
}

void EventManager::setEventCallback(EventCallback callback) {
    eventCallback = callback;
}

void EventManager::printEvents() const {
    Serial.println(F("Scheduled Events:"));
    for (const auto& event : events) {
        uint16_t year;
        uint8_t month, day, hour, minute, second;
        Event::decodeTime(event.timeCode, year, month, day, hour, minute, second);
        Serial.printf("%04d-%02d-%02d %02d:%02d:%02d - Scenario: %d, Cycle: %d, Desc: %s\n",
                      year, month, day, hour, minute, second, event.scenario, event.cycle, event.description.c_str());
    }
}

std::set<EventManager::Event>::const_iterator EventManager::findNextEvent(uint64_t currentTimeCode) const {
    Event searchKey(0, 0, 0, 0, 0, 0, 0, 0);
    searchKey.timeCode = currentTimeCode;
    return events.lower_bound(searchKey);
}

void EventManager::processNextPendingEvent() {
    if (!pendingEvents.empty()) {
        const Event& event = pendingEvents.front();
        currentScenario = event.scenario;
        currentCycle = event.cycle;
        
        // Print event information
        uint16_t year;
        uint8_t month, day, hour, minute, second;
        Event::decodeTime(event.timeCode, year, month, day, hour, minute, second);
        
        // Serial.println("Event Triggered:");
        // Serial.printf("Time: %04d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, minute, second);
        // Serial.printf("Scenario: %d\n", event.scenario);
        // Serial.printf("Cycle: %d\n", event.cycle);
        // Serial.printf("Description: %s\n", event.description.c_str());
        // Serial.println("------------------------------");
        
        if (eventCallback) {
            eventCallback(event);
        }
        
        pendingEvents.pop();
    }
}

bool EventManager::isValidDate(uint16_t year, uint8_t month, uint8_t day) const {
    if (month == 0 || month > 12) return false;
    if (day == 0 || day > 31) return false;
    if (day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)) return false;
    if (month == 2) {
        if (day > 29) return false;
        if (day == 29 && (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))) return false;
    }
    return true;
}