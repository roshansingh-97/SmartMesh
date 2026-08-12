#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
#include "Message.h"
#include "../Utils/CRC16.h"

// Protocol Constants
#define PACKET_START_BYTE 0xAA
#define PACKET_VERSION    0x01
#define MAX_PAYLOAD_SIZE  64

// Struct layout mapped byte-for-byte in memory
struct PacketHeader {
    uint8_t  startByte;
    uint8_t  version;
    uint8_t  messageType;
    uint32_t messageId;
    uint16_t senderId;
    uint16_t receiverId;
    uint32_t timestamp;
    uint8_t  hopCount;
    uint8_t  payloadLength;
} __attribute__((packed));

class Packet {
public:
    // Serialization: Converts Message -> Binary Buffer
    static size_t serialize(const Message& msg, uint8_t* buffer, size_t maxBufferLen);

    // Deserialization: Converts Binary Buffer -> Message
    static bool deserialize(const uint8_t* buffer, size_t bufferLen, Message& outMsg);

    // Validation
    static bool validateHeader(const uint8_t* buffer, size_t bufferLen);
    
    // Calculates total binary size needed for a message
    static size_t getPacketSize(const Message& msg);
};

#endif