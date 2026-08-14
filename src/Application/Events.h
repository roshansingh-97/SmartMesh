#ifndef EVENTS_H
#define EVENTS_H

#include <Arduino.h>

enum class EventType {
    NONE = 0,
    
    // Input Events
    KEY_PRESSED,
    
    // Communication & Network Events
    MESSAGE_RECEIVED,
    MESSAGE_SENT_ACK,
    MESSAGE_SEND_FAILED,
    NODE_JOINED,
    NODE_LEFT,
    
    // Power & System Events
    BATTERY_LOW,
    SCREEN_CHANGE_REQUEST
};

struct KeyPayload {
    char key;
};

struct MessagePayload {
    uint16_t senderId;
    uint8_t messageId;
};

struct NetworkPayload {
    uint32_t nodeId;
};

struct BatteryPayload {
    uint8_t percentage;
};

struct Event {
    EventType type;

    union {
        KeyPayload keyData;
        MessagePayload msgData;
        NetworkPayload netData;
        BatteryPayload batteryData;
        uint16_t rawParam;
    } payload;

    Event() : type(EventType::NONE) {
        payload.rawParam = 0;
    }

    explicit Event(EventType t) : type(t) {
        payload.rawParam = 0;
    }
};

#endif