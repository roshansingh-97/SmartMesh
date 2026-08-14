#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H

#include <Arduino.h>

static const uint8_t MAX_INBOX_SLOTS = 20;
static const uint8_t MAX_SENT_SLOTS = 10;
static const uint8_t MAX_CONTACT_SLOTS = 16;
static const uint8_t MAX_MSG_TEXT_LEN = 140;

struct StoredMessage {
    uint8_t slotId;
    uint16_t senderOrTargetId;
    uint32_t timestamp;
    char text[MAX_MSG_TEXT_LEN + 1];
    bool isRead;

    StoredMessage() : slotId(0), senderOrTargetId(0), timestamp(0), isRead(false) {
        memset(text, 0, sizeof(text));
    }
};

struct UserSettings {
    char nodeName[16];
    uint16_t nodeId;
    bool silentMode;
    bool loopbackMode;

    UserSettings() : nodeId(0x0001), silentMode(false), loopbackMode(false) {
        strncpy(nodeName, "MeshNode", sizeof(nodeName) - 1);
        nodeName[sizeof(nodeName) - 1] = '\0';
    }
};

struct Contact {
    uint16_t nodeId;
    char name[16];

    Contact() : nodeId(0) {
        memset(name, 0, sizeof(name));
    }
};

#endif