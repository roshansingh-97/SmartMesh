#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>
#include "Packet.h"

class Message {
public:
    uint8_t senderID;
    uint8_t receiverID;
    uint16_t msgID;
    String text;
    uint32_t timestamp;

    Message();
    Message(uint8_t sender, uint8_t receiver, uint16_t id, String content);

    Packet toPacket(uint8_t packetType = PACKET_TYPE_TEXT) const;
    static Message fromPacket(const Packet& pkt);
};

#endif