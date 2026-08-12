#ifndef DUPLICATE_FILTER_H
#define DUPLICATE_FILTER_H

#include <Arduino.h>

// Number of recently seen packet signatures to keep in history
#define DUPLICATE_HISTORY_SIZE 20

struct SeenMessageEntry {
    uint16_t senderNodeId;
    uint32_t messageId;
    bool active;
};

class DuplicateFilter {
private:
    SeenMessageEntry history[DUPLICATE_HISTORY_SIZE];
    uint8_t headIndex;
    uint8_t count;

public:
    DuplicateFilter();

    // Query & Update
    bool isDuplicate(uint16_t senderNodeId, uint32_t messageId) const;
    void add(uint16_t senderNodeId, uint32_t messageId);
    
    // Status & Utility
    uint8_t size() const { return count; }
    uint8_t getCapacity() const { return DUPLICATE_HISTORY_SIZE; }
    void clear();
};

#endif