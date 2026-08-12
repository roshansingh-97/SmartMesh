#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <Arduino.h>
#include "Packet.h"

class MessageQueue {
private:
    QueueHandle_t handle;

public:
    MessageQueue();
    bool init(size_t capacity = 10);
    bool push(const Packet& pkt);
    bool pushFromISR(const Packet& pkt, BaseType_t *higherPriorityTaskWoken);
    bool pop(Packet& pkt, uint32_t waitMs = 0);
    bool isEmpty() const;
    void clear();
};

#endif