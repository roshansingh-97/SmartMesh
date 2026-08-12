#include "Packet.h"

size_t Packet::getPacketSize(const Message& msg) {
    uint8_t payloadLen = strnlen(msg.payload, MAX_PAYLOAD_SIZE);
    // Header size + Payload size + 2 Bytes CRC16
    return sizeof(PacketHeader) + payloadLen + sizeof(uint16_t);
}

size_t Packet::serialize(const Message& msg, uint8_t* buffer, size_t maxBufferLen) {
    uint8_t payloadLen = strnlen(msg.payload, MAX_PAYLOAD_SIZE);
    size_t totalSize = sizeof(PacketHeader) + payloadLen + sizeof(uint16_t);

    if (buffer == nullptr || maxBufferLen < totalSize) {
        return 0; // Buffer overflow prevention
    }

    // 1. Populate Header
    PacketHeader* header = (PacketHeader*)buffer;
    header->startByte     = PACKET_START_BYTE;
    header->version       = PACKET_VERSION;
    header->messageType   = static_cast<uint8_t>(msg.messageType);
    header->messageId     = msg.messageId;
    header->senderId      = msg.senderNodeId;
    header->receiverId    = msg.receiverNodeId;
    header->timestamp     = msg.timestamp;
    header->hopCount      = msg.hopCount;
    header->payloadLength = payloadLen;

    // 2. Copy Payload right after Header
    uint8_t* payloadPtr = buffer + sizeof(PacketHeader);
    if (payloadLen > 0) {
        memcpy(payloadPtr, msg.payload, payloadLen);
    }

    // 3. Compute and Append CRC16 over Header + Payload
    uint16_t crc = calculateCRC16(buffer, sizeof(PacketHeader) + payloadLen);
    uint8_t* crcPtr = payloadPtr + payloadLen;
    memcpy(crcPtr, &crc, sizeof(uint16_t));

    return totalSize;
}

bool Packet::validateHeader(const uint8_t* buffer, size_t bufferLen) {
    // 1. Minimum Length Check (Header + CRC minimum)
    if (buffer == nullptr || bufferLen < (sizeof(PacketHeader) + sizeof(uint16_t))) {
        return false;
    }

    const PacketHeader* header = (const PacketHeader*)buffer;

    // 2. Start Byte Check
    if (header->startByte != PACKET_START_BYTE) {
        return false;
    }

    // 3. Protocol Version Check
    if (header->version != PACKET_VERSION) {
        return false;
    }

    // 4. Exact Buffer Length Check vs Header Payload Length declaration
    size_t expectedTotalLen = sizeof(PacketHeader) + header->payloadLength + sizeof(uint16_t);
    if (bufferLen < expectedTotalLen || header->payloadLength > MAX_PAYLOAD_SIZE) {
        return false;
    }

    return true;
}

bool Packet::deserialize(const uint8_t* buffer, size_t bufferLen, Message& outMsg) {
    // Basic validation check
    if (!validateHeader(buffer, bufferLen)) {
        return false;
    }

    const PacketHeader* header = (const PacketHeader*)buffer;

    // Optional CRC Check (integrated with your CRC16 utility)
    size_t payloadAndHeaderLen = sizeof(PacketHeader) + header->payloadLength;
    uint16_t calculatedCRC = calculateCRC16(buffer, payloadAndHeaderLen);
    
    uint16_t receivedCRC = 0;
    memcpy(&receivedCRC, buffer + payloadAndHeaderLen, sizeof(uint16_t));

    if (calculatedCRC != receivedCRC) {
        return false; // CRC Mismatch / Data Corrupted
    }

    // Unpack fields into Message Object
    outMsg.messageId      = header->messageId;
    outMsg.senderNodeId   = header->senderId;
    outMsg.receiverNodeId = header->receiverId;
    outMsg.timestamp      = header->timestamp;
    outMsg.hopCount       = header->hopCount;
    outMsg.messageType    = static_cast<MessageType>(header->messageType);
    outMsg.messageStatus  = STATUS_RECEIVED;

    // Unpack Payload
    memset(outMsg.payload, 0, sizeof(outMsg.payload));
    if (header->payloadLength > 0) {
        memcpy(outMsg.payload, buffer + sizeof(PacketHeader), header->payloadLength);
    }

    return true;
}