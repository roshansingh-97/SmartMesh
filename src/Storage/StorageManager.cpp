#include "StorageManager.h"

// Global singleton instance
StorageManager Storage;

StorageManager::StorageManager() 
    : inboxHead(0), inboxCount(0), sentHead(0), sentCount(0) {}

void StorageManager::begin() {
    loadSettingsInternal();
    loadMetadata();
}

void StorageManager::loadSettingsInternal() {
    prefs.begin("settings", true); // Read-only mode
    if (prefs.isKey("nodeId")) {
        cachedSettings.nodeId = prefs.getUShort("nodeId", 0x0001);
        prefs.getString("nodeName", cachedSettings.nodeName, sizeof(cachedSettings.nodeName));
        cachedSettings.silentMode = prefs.getBool("silent", false);
        cachedSettings.loopbackMode = prefs.getBool("loopback", false);
    } else {
        // First run initialization
        prefs.end();
        saveSettings(cachedSettings);
        return;
    }
    prefs.end();
}

void StorageManager::saveSettings(const UserSettings& newSettings) {
    cachedSettings = newSettings;
    prefs.begin("settings", false);
    prefs.putUShort("nodeId", cachedSettings.nodeId);
    prefs.putString("nodeName", cachedSettings.nodeName);
    prefs.putBool("silent", cachedSettings.silentMode);
    prefs.putBool("loopback", cachedSettings.loopbackMode);
    prefs.end();
}

void StorageManager::loadMetadata() {
    prefs.begin("msg_meta", true);
    inboxHead = prefs.getUChar("ib_head", 0);
    inboxCount = prefs.getUChar("ib_cnt", 0);
    sentHead = prefs.getUChar("st_head", 0);
    sentCount = prefs.getUChar("st_cnt", 0);
    prefs.end();
}

// ================= INBOX OPERATIONS =================

bool StorageManager::saveInboxMessage(uint16_t senderId, const char* text) {
    StoredMessage msg;
    msg.slotId = inboxHead;
    msg.senderOrTargetId = senderId;
    msg.timestamp = millis() / 1000;
    msg.isRead = false;
    strncpy(msg.text, text, MAX_MSG_TEXT_LEN);
    msg.text[MAX_MSG_TEXT_LEN] = '\0';

    char key[12];
    snprintf(key, sizeof(key), "in_%d", inboxHead);

    prefs.begin("inbox", false);
    prefs.putBytes(key, &msg, sizeof(StoredMessage));
    prefs.end();

    // Advance ring buffer pointer
    inboxHead = (inboxHead + 1) % MAX_INBOX_SLOTS;
    if (inboxCount < MAX_INBOX_SLOTS) {
        inboxCount++;
    }

    // Persist updated metadata
    prefs.begin("msg_meta", false);
    prefs.putUChar("ib_head", inboxHead);
    prefs.putUChar("ib_cnt", inboxCount);
    prefs.end();

    return true;
}

bool StorageManager::getInboxMessage(uint8_t index, StoredMessage& outMsg) {
    if (index >= inboxCount) return false;

    // Calculate physical slot from relative index (newest first)
    int16_t slot = (int16_t)inboxHead - 1 - index;
    if (slot < 0) {
        slot += MAX_INBOX_SLOTS;
    }

    char key[12];
    snprintf(key, sizeof(key), "in_%d", slot);

    prefs.begin("inbox", true);
    size_t len = prefs.getBytes(key, &outMsg, sizeof(StoredMessage));
    prefs.end();

    return (len == sizeof(StoredMessage));
}

void StorageManager::markInboxRead(uint8_t index) {
    StoredMessage msg;
    if (getInboxMessage(index, msg)) {
        msg.isRead = true;
        char key[12];
        snprintf(key, sizeof(key), "in_%d", msg.slotId);

        prefs.begin("inbox", false);
        prefs.putBytes(key, &msg, sizeof(StoredMessage));
        prefs.end();
    }
}

void StorageManager::clearInbox() {
    prefs.begin("inbox", false);
    prefs.clear();
    prefs.end();

    inboxHead = 0;
    inboxCount = 0;

    prefs.begin("msg_meta", false);
    prefs.putUChar("ib_head", 0);
    prefs.putUChar("ib_cnt", 0);
    prefs.end();
}

// ================= SENT OPERATIONS =================

bool StorageManager::saveSentMessage(uint16_t targetId, const char* text) {
    StoredMessage msg;
    msg.slotId = sentHead;
    msg.senderOrTargetId = targetId;
    msg.timestamp = millis() / 1000;
    msg.isRead = true;
    strncpy(msg.text, text, MAX_MSG_TEXT_LEN);
    msg.text[MAX_MSG_TEXT_LEN] = '\0';

    char key[12];
    snprintf(key, sizeof(key), "st_%d", sentHead);

    prefs.begin("sent", false);
    prefs.putBytes(key, &msg, sizeof(StoredMessage));
    prefs.end();

    sentHead = (sentHead + 1) % MAX_SENT_SLOTS;
    if (sentCount < MAX_SENT_SLOTS) {
        sentCount++;
    }

    prefs.begin("msg_meta", false);
    prefs.putUChar("st_head", sentHead);
    prefs.putUChar("st_cnt", sentCount);
    prefs.end();

    return true;
}

bool StorageManager::getSentMessage(uint8_t index, StoredMessage& outMsg) {
    if (index >= sentCount) return false;

    int16_t slot = (int16_t)sentHead - 1 - index;
    if (slot < 0) {
        slot += MAX_SENT_SLOTS;
    }

    char key[12];
    snprintf(key, sizeof(key), "st_%d", slot);

    prefs.begin("sent", true);
    size_t len = prefs.getBytes(key, &outMsg, sizeof(StoredMessage));
    prefs.end();

    return (len == sizeof(StoredMessage));
}

void StorageManager::clearSent() {
    prefs.begin("sent", false);
    prefs.clear();
    prefs.end();

    sentHead = 0;
    sentCount = 0;

    prefs.begin("msg_meta", false);
    prefs.putUChar("st_head", 0);
    prefs.putUChar("st_cnt", 0);
    prefs.end();
}

// ================= CONTACT OPERATIONS =================

const char* StorageManager::getContactName(uint16_t nodeId, char* outName, size_t maxLen) {
    char key[12];
    snprintf(key, sizeof(key), "c_%04X", nodeId);

    prefs.begin("contacts", true);
    if (prefs.isKey(key)) {
        prefs.getString(key, outName, maxLen);
    } else {
        snprintf(outName, maxLen, "Node 0x%04X", nodeId);
    }
    prefs.end();
    return outName;
}

bool StorageManager::saveContact(uint16_t nodeId, const char* name) {
    char key[12];
    snprintf(key, sizeof(key), "c_%04X", nodeId);

    prefs.begin("contacts", false);
    size_t len = prefs.putString(key, name);
    prefs.end();

    return len > 0;
}