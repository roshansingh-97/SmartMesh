#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>
#include "../Config.h"

// Enums for Message Classification
enum MessageType : uint8_t {
    MSG_TYPE_TEXT = 0,
    MSG_TYPE_ACK,
    MSG_TYPE_TELEMETRY,
    MSG_TYPE_PING,
    MSG_TYPE_ROUTING
};

// Enums for Message Delivery Tracking
enum MessageStatus : uint8_t {
    STATUS_UNKNOWN = 0,
    STATUS_PENDING,
    STATUS_SENT,
    STATUS_DELIVERED,
    STATUS_FAILED,
    STATUS_RECEIVED
};

class Message {
public:
    uint16_t messageId;
    uint8_t senderNodeId;
    uint8_t receiverNodeId;
    uint32_t timestamp;
    uint8_t hopCount;
    MessageType messageType;
    MessageStatus messageStatus;
    char payload[64];

    // Constructors
    Message();
    Message(uint8_t sender, uint8_t receiver, uint16_t msgId, const char* text, MessageType type = MSG_TYPE_TEXT);

    // Helpers
    void incrementHop();
    bool isBroadcast() const;
    
    // UI String Converters
    const char* getTypeString() const;
    const char* getStatusString() const;

    // Conversion to/from raw network packet
    SmartMeshPacket toRawPacket() const;
    static Message fromRawPacket(const SmartMeshPacket& raw);
};

#endif