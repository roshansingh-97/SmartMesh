#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include "Events.h"

class EventQueue {
private:
    static const uint8_t QUEUE_CAPACITY = 16;
    Event queue[QUEUE_CAPACITY];
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;

public:
    EventQueue() : head(0), tail(0), count(0) {}

    bool push(const Event& event) {
        if (count >= QUEUE_CAPACITY) {
            // Queue full - drop event to protect buffer integrity
            return false;
        }
        queue[tail] = event;
        tail = (tail + 1) % QUEUE_CAPACITY;
        count++;
        return true;
    }

    bool pop(Event& outEvent) {
        if (count == 0) {
            return false;
        }
        outEvent = queue[head];
        head = (head + 1) % QUEUE_CAPACITY;
        count--;
        return true;
    }

    bool isEmpty() const {
        return count == 0;
    }

    uint8_t size() const {
        return count;
    }

    void clear() {
        head = 0;
        tail = 0;
        count = 0;
    }
};

#endif