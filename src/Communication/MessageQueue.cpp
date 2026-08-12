#include "MessageQueue.h"

MessageQueue::MessageQueue() {
    clear();
}

bool MessageQueue::enqueue(const Message& msg) {
    if (isFull()) {
        return false; // Overflow prevented
    }

    buffer[tail] = msg;
    tail = (tail + 1) % QUEUE_CAPACITY;
    count++;
    return true;
}

bool MessageQueue::dequeue(Message& outMsg) {
    if (isEmpty()) {
        return false; // Underflow prevented
    }

    outMsg = buffer[head];
    head = (head + 1) % QUEUE_CAPACITY;
    count--;
    return true;
}

bool MessageQueue::peek(Message& outMsg) const {
    if (isEmpty()) {
        return false;
    }

    outMsg = buffer[head];
    return true;
}

bool MessageQueue::isEmpty() const {
    return (count == 0);
}

bool MessageQueue::isFull() const {
    return (count == QUEUE_CAPACITY);
}

uint8_t MessageQueue::size() const {
    return count;
}

void MessageQueue::clear() {
    head = 0;
    tail = 0;
    count = 0;
}