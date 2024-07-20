// EventManager.h
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <set>
#include <queue>
#include <functional>
#include <cstdint>
#include <Arduino.h>
#include "RTClib.h"

class EventManager {
public:
    struct Event {
        uint64_t timeCode;
        uint8_t scenario;
        uint8_t cycle;
        String description;
        bool isDaily;

        Event(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, 
              uint8_t sc, uint8_t cy, const String& desc = "", bool daily = false)
            : timeCode(encodeTime(year, month, day, hour, minute, second)), 
              scenario(sc), cycle(cy), description(desc), isDaily(daily) {}

        bool operator<(const Event& other) const {
            return timeCode < other.timeCode;
        }

        static uint64_t encodeTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
            return ((uint64_t)year << 40) | ((uint64_t)month << 32) | ((uint64_t)day << 24) | 
                   ((uint64_t)hour << 16) | ((uint64_t)minute << 8) | second;
        }

        static void decodeTime(uint64_t timeCode, uint16_t& year, uint8_t& month, uint8_t& day, 
                               uint8_t& hour, uint8_t& minute, uint8_t& second) {
            year = (timeCode >> 40) & 0xFFFF;
            month = (timeCode >> 32) & 0xFF;
            day = (timeCode >> 24) & 0xFF;
            hour = (timeCode >> 16) & 0xFF;
            minute = (timeCode >> 8) & 0xFF;
            second = timeCode & 0xFF;
        }
    };

    using EventCallback = std::function<void(const Event&)>;

    EventManager(RTC_DS3231& rtc);
    void begin();
    bool addEvent(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, 
                  uint8_t scenario, uint8_t cycle, const String& description = "", bool isDaily = false);
    bool removeEvent(uint64_t timeCode);
    void clearEvents();
    void update();
    uint8_t getCurrentScenario() const;
    uint8_t getCurrentCycle() const;
    void setEventCallback(EventCallback callback);
    void printEvents() const;

private:
    std::set<Event> events;
    std::queue<Event> pendingEvents;
    RTC_DS3231& rtc;
    uint8_t currentScenario;
    uint8_t currentCycle;
    EventCallback eventCallback;
    volatile bool isProcessingEvents;
    unsigned long lastProcessTime;

    std::set<Event>::const_iterator findNextEvent(uint64_t currentTimeCode) const;
    void processNextPendingEvent();
    bool isValidDate(uint16_t year, uint8_t month, uint8_t day) const;
    void rescheduleEvent(const Event& event);
    void addDailyEvent(uint8_t hour, uint8_t minute, uint8_t second, 
                       uint8_t scenario, uint8_t cycle, const String& description);
};

#endif // EVENT_MANAGER_H