#include "DuplicateFilter.h"

DuplicateFilter::DuplicateFilter() : headIndex(0) {
    clear();
}

void DuplicateFilter::begin() {
    clear();
}

void DuplicateFilter::clear() {
    for (uint8_t i = 0; i < HISTORY_SIZE; i++) {
        history[i].senderID = 0;
        history[i].msgID = 0;
    }
    headIndex = 0;
}

bool DuplicateFilter::isDuplicate(uint8_t senderID, uint16_t msgID) {
    for (uint8_t i = 0; i < HISTORY_SIZE; i++) {
        if (history[i].senderID == senderID && history[i].msgID == msgID) {
            return true;
        }
    }
    return false;
}

void DuplicateFilter::add(uint8_t senderID, uint16_t msgID) {
    if (isDuplicate(senderID, msgID)) return;

    history[headIndex].senderID = senderID;
    history[headIndex].msgID = msgID;
    headIndex = (headIndex + 1) % HISTORY_SIZE;
}