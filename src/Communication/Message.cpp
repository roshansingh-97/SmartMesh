#include "Message.h"

Message::Message() 
    : messageId(0),
      senderNodeId(0),
      receiverNodeId(0),
      timestamp(0),
      hopCount(0),
      messageType(MSG_TYPE_TEXT),
      messageStatus(STATUS_UNKNOWN) {
    memset(payload, 0, sizeof(payload));
}

Message::Message(uint8_t sender, uint8_t receiver, uint16_t msgId, const char* text, MessageType type)
    : messageId(msgId),
      senderNodeId(sender),
      receiverNodeId(receiver),
      timestamp(millis()),
      hopCount(0),
      messageType(type),
      messageStatus(STATUS_PENDING) {
    memset(payload, 0, sizeof(payload));
    if (text) {
        strncpy(payload, text, sizeof(payload) - 1);
    }
}

void Message::incrementHop() {
    if (hopCount < 255) {
        hopCount++;
    }
}

bool Message::isBroadcast() const {
    return receiverNodeId == 0xFF; // 0xFF designates a broadcast address
}

const char* Message::getTypeString() const {
    switch (messageType) {
        case MSG_TYPE_TEXT:      return "TEXT";
        case MSG_TYPE_ACK:       return "ACK";
        case MSG_TYPE_TELEMETRY: return "TELEMETRY";
        case MSG_TYPE_PING:      return "PING";
        case MSG_TYPE_ROUTING:   return "ROUTING";
        default:                 return "UNKNOWN";
    }
}

const char* Message::getStatusString() const {
    switch (messageStatus) {
        case STATUS_PENDING:   return "Pending";
        case STATUS_SENT:      return "Sent";
        case STATUS_DELIVERED: return "Delivered";
        case STATUS_FAILED:    return "Failed";
        case STATUS_RECEIVED:  return "Received";
        default:               return "Unknown";
    }
}

SmartMeshPacket Message::toRawPacket() const {
    SmartMeshPacket pkt;
    memset(&pkt, 0, sizeof(SmartMeshPacket));
    pkt.senderID = senderNodeId;
    pkt.receiverID = receiverNodeId;
    pkt.msgID = messageId;
    pkt.packetType = static_cast<uint8_t>(messageType);
    strncpy(pkt.payload, payload, sizeof(pkt.payload) - 1);
    return pkt;
}

Message Message::fromRawPacket(const SmartMeshPacket& raw) {
    Message msg;
    msg.senderNodeId = raw.senderID;
    msg.receiverNodeId = raw.receiverID;
    msg.messageId = raw.msgID;
    msg.messageType = static_cast<MessageType>(raw.packetType);
    msg.timestamp = millis();
    msg.hopCount = 0;
    msg.messageStatus = STATUS_RECEIVED;
    strncpy(msg.payload, raw.payload, sizeof(msg.payload) - 1);
    return msg;
}