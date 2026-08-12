#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <Arduino.h>
#include "Message.h"

// Maximum messages that can be queued in memory at once
#define QUEUE_CAPACITY 10

class MessageQueue {
private:
    Message buffer[QUEUE_CAPACITY];
    uint8_t head;
    uint8_t tail;
    uint8_t count;

public:
    MessageQueue();

    // Queue Operations
    bool enqueue(const Message& msg);
    bool dequeue(Message& outMsg);
    bool peek(Message& outMsg) const;

    // Status Queries
    bool isEmpty() const;
    bool isFull() const;
    uint8_t size() const;
    uint8_t getCapacity() const { return QUEUE_CAPACITY; }

    // Maintenance
    void clear();
};

#endif