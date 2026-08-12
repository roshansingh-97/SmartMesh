#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <Arduino.h>
#include "Message.h"
#include "Packet.h"
#include "MessageQueue.h"
#include "DuplicateFilter.h"
#include "AckManager.h"
#include "../Radio/MeshRadio.h"

class CommunicationManager {
private:
    MessageQueue txQueue;
    DuplicateFilter dupFilter;
    AckManager ackManager;
    MeshRadio* radio;

    // Internal pipeline helpers
    void processOutgoing();
    void processTimeouts();
    void processIncoming();

public:
    CommunicationManager();

    void begin(MeshRadio* radioInstance);
    
    // High-Level Application API
    bool sendMessage(uint8_t receiverId, const char* text, MessageType type = MSG_TYPE_TEXT);
    
    // Non-blocking main tick run inside loop()
    void process();

    // Stats and Status Queries
    uint8_t getPendingTxCount() const { return txQueue.size(); }
    uint8_t getPendingAckCount() const { return ackManager.getPendingCount(); }
};

#endif