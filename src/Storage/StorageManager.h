#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Preferences.h>
#include "StorageTypes.h"

class StorageManager {
private:
    Preferences prefs;
    UserSettings cachedSettings;

    uint8_t inboxHead;
    uint8_t inboxCount;

    uint8_t sentHead;
    uint8_t sentCount;

    void loadSettingsInternal();
    void loadMetadata();

public:
    StorageManager();

    void begin();

    // 1. Settings Operations
    const UserSettings& getSettings() const { return cachedSettings; }
    void saveSettings(const UserSettings& newSettings);

    // 2. Inbox Message Operations
    bool saveInboxMessage(uint16_t senderId, const char* text);
    uint8_t getInboxCount() const { return inboxCount; }
    bool getInboxMessage(uint8_t index, StoredMessage& outMsg);
    void markInboxRead(uint8_t index);
    void clearInbox();

    // 3. Sent Message Operations
    bool saveSentMessage(uint16_t targetId, const char* text);
    uint8_t getSentCount() const { return sentCount; }
    bool getSentMessage(uint8_t index, StoredMessage& outMsg);
    void clearSent();

    // 4. Contact Operations
    const char* getContactName(uint16_t nodeId, char* outName, size_t maxLen);
    bool saveContact(uint16_t nodeId, const char* name);
};

extern StorageManager Storage;

#endif