#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- HARDWARE PIN DEFINITIONS ---
#define OLED_SDA        21
#define OLED_SCL        22

#define KEYPAD_R1       13
#define KEYPAD_R2       12
#define KEYPAD_R3       14
#define KEYPAD_R4       27
#define KEYPAD_C1       26
#define KEYPAD_C2       25
#define KEYPAD_C3       33
#define KEYPAD_C4       32

#define BUZZER_PIN      15
#define BATTERY_ADC_PIN 34

// --- NETWORK CONFIGURATION ---
#define WIFI_CHANNEL    1
#define NODE_ID         0x01
#define MAX_PAYLOAD_LEN 64

// Packet Types
#define PACKET_TYPE_DATA 0
#define PACKET_TYPE_ACK  1

// --- PACKET STRUCTURE ---
struct __attribute__((packed)) SmartMeshPacket {
    uint8_t  senderID;
    uint8_t  receiverID;
    uint8_t  packetType;   // 0 = DATA, 1 = ACK
    uint16_t msgID;        // Matches msgID used in MeshRadio
    char     payload[MAX_PAYLOAD_LEN];
    uint16_t crc;          // CRC field placed last for CRC calculation
};

// --- SYSTEM STATES ---
enum SystemState {
    STATE_BOOT,
    STATE_HOME_MENU,
    STATE_COMPOSE,
    STATE_INBOX,
    STATE_SENT,
    STATE_CONTACTS,
    STATE_SETTINGS
};

// --- EXTERN DECLARATIONS ---
extern const uint8_t broadcastMAC[6];

#endif // CONFIG_H