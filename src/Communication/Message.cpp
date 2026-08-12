#include "Message.h"

Message::Message() : senderID(0), receiverID(0), msgID(0), text(""), timestamp(0) {}

Message::Message(uint8_t sender, uint8_t receiver, uint16_t id, String content)
    : senderID(sender), receiverID(receiver), msgID(id), text(content), timestamp(millis()) {}

Packet Message::toPacket(uint8_t packetType) const {
    return Packet(senderID, receiverID, msgID, packetType, text.c_str());
}

Message Message::fromPacket(const Packet& pkt) {
    return Message(pkt.rawPacket.senderID, pkt.rawPacket.receiverID, pkt.rawPacket.msgID, String(pkt.rawPacket.payload));
}