#include "CommunicationManager.h"

CommunicationManager::CommunicationManager() : radio(nullptr) {}

void CommunicationManager::begin(MeshRadio* radioInstance) {
    radio = radioInstance;
    txQueue.clear();
    dupFilter.clear();
    ackManager.begin();
}

bool CommunicationManager::sendMessage(uint8_t receiverId, const char* text, MessageType type) {
    uint16_t msgId = static_cast<uint16_t>(millis() & 0xFFFF);
    Message msg(NODE_ID, receiverId, msgId, text, type);
    
    return txQueue.enqueue(msg);
}

void CommunicationManager::processOutgoing() {
    if (!txQueue.isEmpty()) {
        Message msg;
        if (txQueue.peek(msg)) {
            // Convert to binary packet
            uint8_t buffer[128];
            size_t packetSize = Packet::serialize(msg, buffer, sizeof(buffer));

            if (packetSize > 0 && radio != nullptr) {
                // Transmit over radio hardware
                SmartMeshPacket rawPkt = msg.toRawPacket();
                if (radio->sendWithRetry(rawPkt)) { // Temporary hardware trigger
                    txQueue.dequeue(msg); // Remove from queue on successful send
                    
                    if (!msg.isBroadcast()) {
                        ackManager.addPending(msg); // Track for delivery acknowledgment
                    }
                }
            }
        }
    }
}

void CommunicationManager::processTimeouts() {
    Message msgToRetry;
    if (ackManager.checkTimeouts(msgToRetry)) {
        // Re-enqueue message for re-transmission upon timeout
        txQueue.enqueue(msgToRetry);
    }
}

void CommunicationManager::processIncoming() {
    SmartMeshPacket rawPkt;

    if (xQueueReceive(MeshRadio::xQueueIncoming, &rawPkt, 0) == pdTRUE) {
        Message receivedMsg = Message::fromRawPacket(rawPkt);

        // 1. Duplicate Filter Check
        if (dupFilter.isDuplicate(receivedMsg.senderNodeId, receivedMsg.messageId)) {
            return; // Ignore duplicate packets
        }
        dupFilter.add(receivedMsg.senderNodeId, receivedMsg.messageId);

        // 2. Process Received ACK
        if (receivedMsg.messageType == MSG_TYPE_ACK) {
            ackManager.processAck(receivedMsg.messageId, receivedMsg.senderNodeId);
            return;
        }

        // 3. Send ACK back if packet was addressed specifically to this node
        if (receivedMsg.receiverNodeId == NODE_ID && receivedMsg.messageType != MSG_TYPE_ACK) {
            if (radio != nullptr) {
                radio->sendACK(receivedMsg.messageId, receivedMsg.senderNodeId);
            }
        }

        // 4. Pass message to storage/UI (handshake complete)
        // Handled via application callback or shared buffer
    }
}

void CommunicationManager::process() {
    processIncoming();
    processOutgoing();
    processTimeouts();
}