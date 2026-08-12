#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <Preferences.h>
#include "Config.h"

#define MAX_SAVED_MESSAGES 50

class StorageManager {
private:
    Preferences prefs;
    uint8_t writeIndex = 0;

public:
    void begin();
    void saveMessage(const SmartMeshPacket& packet);
    bool readMessage(uint8_t index, SmartMeshPacket& packet);
    uint8_t getSavedCount();
    void clearAll();
};

extern StorageManager Storage;

#endif