#ifndef DUPLICATE_FILTER_H
#define DUPLICATE_FILTER_H

#include <Arduino.h>

struct SeenPacketEntry {
    uint8_t senderID;
    uint16_t msgID;
};

class DuplicateFilter {
private:
    static const uint8_t HISTORY_SIZE = 20;
    SeenPacketEntry history[HISTORY_SIZE];
    uint8_t headIndex;

public:
    DuplicateFilter();
    void begin();
    bool isDuplicate(uint8_t senderID, uint16_t msgID);
    void add(uint8_t senderID, uint16_t msgID);
    void clear();
};

#endif