#include "Packet.h"

Packet::Packet() {
    memset(&rawPacket, 0, sizeof(SmartMeshPacket));
}

Packet::Packet(uint8_t sender, uint8_t receiver, uint16_t msgID, uint8_t type, const char* payload) {
    memset(&rawPacket, 0, sizeof(SmartMeshPacket));
    rawPacket.senderID = sender;
    rawPacket.receiverID = receiver;
    rawPacket.msgID = msgID;
    rawPacket.packetType = type;
    if (payload) {
        strncpy(rawPacket.payload, payload, sizeof(rawPacket.payload) - 1);
    }
    updateCRC();
}

bool Packet::isValidCRC() const {
    uint16_t calculated = calculateCRC16((const uint8_t*)&rawPacket, sizeof(SmartMeshPacket) - sizeof(uint16_t));
    return (rawPacket.crc == calculated);
}

void Packet::updateCRC() {
    rawPacket.crc = calculateCRC16((const uint8_t*)&rawPacket, sizeof(SmartMeshPacket) - sizeof(uint16_t));
}