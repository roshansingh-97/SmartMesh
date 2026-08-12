#include "Storage.h"

StorageManager Storage;

void StorageManager::begin() {
    prefs.begin("smartmesh", false);
    writeIndex = prefs.getUChar("wIdx", 0);
}

void StorageManager::saveMessage(const SmartMeshPacket& packet) {
    char key[10];
    snprintf(key, sizeof(key), "msg_%d", writeIndex);
    
    prefs.putBytes(key, &packet, sizeof(SmartMeshPacket));
    
    writeIndex = (writeIndex + 1) % MAX_SAVED_MESSAGES;
    prefs.putUChar("wIdx", writeIndex);
    
    uint8_t count = prefs.getUChar("count", 0);
    if (count < MAX_SAVED_MESSAGES) {
        prefs.putUChar("count", count + 1);
    }
}

bool StorageManager::readMessage(uint8_t index, SmartMeshPacket& packet) {
    uint8_t count = getSavedCount();
    if (index >= count) return false;
    
    char key[10];
    snprintf(key, sizeof(key), "msg_%d", index);
    
    size_t bytesRead = prefs.getBytes(key, &packet, sizeof(SmartMeshPacket));
    return (bytesRead == sizeof(SmartMeshPacket));
}

uint8_t StorageManager::getSavedCount() {
    return prefs.getUChar("count", 0);
}

void StorageManager::clearAll() {
    prefs.clear();
    writeIndex = 0;
}