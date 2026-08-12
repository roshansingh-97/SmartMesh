#include "AckManager.h"

AckManager::AckManager(uint32_t timeoutMs) : ackTimeoutMs(timeoutMs) {
    clear();
}

void AckManager::begin() {
    clear();
}

void AckManager::clear() {
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        pendingList[i].active = false;
    }
}

bool AckManager::addPending(const Packet& pkt, uint8_t maxRetries) {
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        if (!pendingList[i].active) {
            pendingList[i].packet = pkt;
            pendingList[i].lastSentTime = millis();
            pendingList[i].retriesLeft = maxRetries;
            pendingList[i].active = true;
            return true;
        }
    }
    return false; // Queue full
}

bool AckManager::handleAckReceived(uint16_t msgID, uint8_t senderID) {
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        if (pendingList[i].active && 
            pendingList[i].packet.rawPacket.msgID == msgID &&
            pendingList[i].packet.rawPacket.receiverID == senderID) {
            pendingList[i].active = false; // Acknowledged & resolved
            return true;
        }
    }
    return false;
}

bool AckManager::checkTimeouts(Packet& pktToRetry) {
    uint32_t now = millis();
    for (uint8_t i = 0; i < MAX_PENDING; i++) {
        if (pendingList[i].active) {
            if (now - pendingList[i].lastSentTime >= ackTimeoutMs) {
                if (pendingList[i].retriesLeft > 0) {
                    pendingList[i].retriesLeft--;
                    pendingList[i].lastSentTime = now;
                    pktToRetry = pendingList[i].packet;
                    return true; // Requires re-transmission
                } else {
                    pendingList[i].active = false; // Retries exhausted
                }
            }
        }
    }
    return false;
}