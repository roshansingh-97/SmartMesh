#include "AckManager.h"

AckManager::AckManager(uint32_t ackTimeoutMs) : timeoutMs(ackTimeoutMs) {
    clear();
}

void AckManager::begin() {
    clear();
}

void AckManager::clear() {
    for (uint8_t i = 0; i < MAX_PENDING_ACK; i++) {
        pendingList[i].active = false;
        pendingList[i].retryCount = 0;
        pendingList[i].maxRetries = DEFAULT_MAX_RETRIES;
        pendingList[i].lastSentTime = 0;
    }
}

bool AckManager::addPending(const Message& msg, uint8_t maxRetries) {
    // Broadcast messages (0xFF) do not expect ACKs
    if (msg.isBroadcast()) {
        return false;
    }

    for (uint8_t i = 0; i < MAX_PENDING_ACK; i++) {
        if (!pendingList[i].active) {
            pendingList[i].msg = msg;
            pendingList[i].msg.messageStatus = STATUS_SENT;
            pendingList[i].lastSentTime = millis();
            pendingList[i].retryCount = 0;
            pendingList[i].maxRetries = maxRetries;
            pendingList[i].active = true;
            return true;
        }
    }
    return false; // Queue full
}

bool AckManager::processAck(uint16_t messageId, uint8_t senderNodeId) {
    for (uint8_t i = 0; i < MAX_PENDING_ACK; i++) {
        if (pendingList[i].active && 
            pendingList[i].msg.messageId == messageId && 
            pendingList[i].msg.receiverNodeId == senderNodeId) {
            
            pendingList[i].msg.messageStatus = STATUS_DELIVERED;
            pendingList[i].active = false; // Resolved
            return true;
        }
    }
    return false; // Not found or already resolved
}

bool AckManager::checkTimeouts(Message& msgToRetry) {
    uint32_t now = millis();

    for (uint8_t i = 0; i < MAX_PENDING_ACK; i++) {
        if (pendingList[i].active) {
            // Non-blocking timer check
            if (now - pendingList[i].lastSentTime >= timeoutMs) {
                if (pendingList[i].retryCount < pendingList[i].maxRetries) {
                    // Timeout hit -> trigger retry
                    pendingList[i].retryCount++;
                    pendingList[i].lastSentTime = now;
                    pendingList[i].msg.incrementHop(); // Optionally track transmission attempts
                    msgToRetry = pendingList[i].msg;
                    return true;
                } else {
                    // Maximum retries exceeded -> mark as failed
                    pendingList[i].msg.messageStatus = STATUS_FAILED;
                    pendingList[i].active = false;
                }
            }
        }
    }
    return false;
}

bool AckManager::hasPending() const {
    return getPendingCount() > 0;
}

uint8_t AckManager::getPendingCount() const {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MAX_PENDING_ACK; i++) {
        if (pendingList[i].active) {
            count++;
        }
    }
    return count;
}