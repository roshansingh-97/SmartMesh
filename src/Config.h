#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define NODE_ID 1

// Pin Definitions
#define OLED_SCL 22
#define OLED_SDA 21

#define KEYPAD_R1 13
#define KEYPAD_R2 12
#define KEYPAD_R3 14
#define KEYPAD_R4 27
#define KEYPAD_C1 26
#define KEYPAD_C2 25
#define KEYPAD_C3 33
#define KEYPAD_C4 32

// Packet Types
enum PacketType {
    PACKET_TYPE_ACK,
    PACKET_TYPE_TEXT,
    PACKET_TYPE_TELEMETRY
};

// Application States
enum SystemState {
    STATE_MENU,
    STATE_COMPOSE,
    STATE_INBOX
};

// Data Structure
struct SmartMeshPacket {
    uint8_t senderID;
    uint8_t receiverID;
    uint16_t msgID;
    uint8_t packetType;
    char payload[64];
    uint16_t crc;
};

#endif