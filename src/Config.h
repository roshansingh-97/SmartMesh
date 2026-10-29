#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Communication Modes
enum CommunicationMode : uint8_t {
    MODE_1_PHONE_TO_PHONE = 1,  // Phone A -> SCT 1 -> SCT 2 -> Phone B
    MODE_2_SCT_TO_SCT     = 2,  // Standalone Keypad -> SCT 2 Screen
    MODE_3_PHONE_TO_SCT   = 3,  // Phone A -> SCT 1 -> SCT 2 Screen
    MODE_4_SCT_TO_PHONE   = 4   // Standalone Keypad -> SCT 2 -> Phone B
};

// Packet Types
enum PacketType : uint8_t {
    PACKET_TYPE_ACK       = 0x01,
    PACKET_TYPE_TEXT      = 0x02,
    PACKET_TYPE_TELEMETRY = 0x03
};

// Application States
enum SystemState {
    SCREEN_SPLASH,
    STATE_MENU,
    STATE_COMPOSE,
    STATE_INBOX
};

// Default Node Configuration
#define NODE_ID 0x0001
#define MAX_PAYLOAD_LEN 64

// Pin Definitions
#define OLED_SCL 22
#define OLED_SDA 21

// Keypad Pins
#define KEYPAD_R1 13
#define KEYPAD_R2 18
#define KEYPAD_R3 14
#define KEYPAD_R4 27
#define KEYPAD_C1 26
#define KEYPAD_C2 25
#define KEYPAD_C3 32  // Swapped C3/C4 back
#define KEYPAD_C4 33  // Swapped C3/C4 back

// Universal Radio Packet
struct __attribute__((packed)) SmartMeshPacket {
    uint8_t mode;               
    uint8_t packetType;         
    uint16_t msgID;             
    uint16_t senderID;          
    uint16_t receiverID;        
    char payload[MAX_PAYLOAD_LEN]; 
    uint16_t crc;               
};

#endif // CONFIG_H