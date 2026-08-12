#ifndef ACK_MANAGER_H
#define ACK_MANAGER_H

#include <Arduino.h>
#include "Packet.h"

struct PendingAck {
    Packet packet;
    uint32_t lastSentTime;
    uint8_t retriesLeft;
    bool active;
};

class AckManager {
private:
    static const uint8_t MAX_PENDING = 5;
    PendingAck pendingList[MAX_PENDING];
    uint32_t ackTimeoutMs;

public:
    AckManager(uint32_t timeoutMs = 300);
    void begin();
    bool addPending(const Packet& pkt, uint8_t maxRetries = 3);
    bool handleAckReceived(uint16_t msgID, uint8_t senderID);
    bool checkTimeouts(Packet& pktToRetry);
    void clear();
};

#endif