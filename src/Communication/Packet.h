#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
#include "../Config.h"
#include "../Utils/CRC16.h"

class Packet {
public:
    SmartMeshPacket rawPacket;

    Packet();
    Packet(uint8_t sender, uint8_t receiver, uint16_t msgID, uint8_t type, const char* payload);

    bool isValidCRC() const;
    void updateCRC();
    size_t getSize() const { return sizeof(SmartMeshPacket); }
};

#endif