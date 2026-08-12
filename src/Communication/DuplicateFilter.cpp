#include "DuplicateFilter.h"

DuplicateFilter::DuplicateFilter() {
    clear();
}

void DuplicateFilter::clear() {
    for (uint8_t i = 0; i < DUPLICATE_HISTORY_SIZE; i++) {
        history[i].senderNodeId = 0;
        history[i].messageId = 0;
        history[i].active = false;
    }
    headIndex = 0;
    count = 0;
}

bool DuplicateFilter::isDuplicate(uint16_t senderNodeId, uint32_t messageId) const {
    for (uint8_t i = 0; i < DUPLICATE_HISTORY_SIZE; i++) {
        if (history[i].active && 
            history[i].senderNodeId == senderNodeId && 
            history[i].messageId == messageId) {
            return true; // Duplicate found
        }
    }
    return false; // Unique / Not seen
}

void DuplicateFilter::add(uint16_t senderNodeId, uint32_t messageId) {
    // If it's already in history, do not waste ring-buffer slots updating it
    if (isDuplicate(senderNodeId, messageId)) {
        return;
    }

    // Insert at current head index (overwriting oldest if buffer is full)
    history[headIndex].senderNodeId = senderNodeId;
    history[headIndex].messageId = messageId;
    history[headIndex].active = true;

    // Advance ring-buffer head
    headIndex = (headIndex + 1) % DUPLICATE_HISTORY_SIZE;

    if (count < DUPLICATE_HISTORY_SIZE) {
        count++;
    }
}