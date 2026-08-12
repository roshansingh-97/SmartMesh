#ifndef ACK_MANAGER_H
#define ACK_MANAGER_H

#include <Arduino.h>
#include "Message.h"

#define MAX_PENDING_ACK 5
#define DEFAULT_ACK_TIMEOUT_MS 3000
#define DEFAULT_MAX_RETRIES 3

struct PendingMessage {
    Message msg;
    uint32_t lastSentTime;
    uint8_t retryCount;
    uint8_t maxRetries;
    bool active;
};

class AckManager {
private:
    PendingMessage pendingList[MAX_PENDING_ACK];
    uint32_t timeoutMs;

public:
    AckManager(uint32_t ackTimeoutMs = DEFAULT_ACK_TIMEOUT_MS);

    // Core Lifecycle
    void begin();
    bool addPending(const Message& msg, uint8_t maxRetries = DEFAULT_MAX_RETRIES);
    bool processAck(uint16_t messageId, uint8_t senderNodeId);

    // Timeout & Retries Engine
    // Returns true if a message needs retransmission, populating msgToRetry
    bool checkTimeouts(Message& msgToRetry);

    // Queries
    bool hasPending() const;
    uint8_t getPendingCount() const;
    
    // Maintenance
    void clear();
};

#endif