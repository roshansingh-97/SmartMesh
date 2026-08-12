#include "MessageQueue.h"

MessageQueue::MessageQueue() : handle(NULL) {}

bool MessageQueue::init(size_t capacity) {
    handle = xQueueCreate(capacity, sizeof(SmartMeshPacket));
    return (handle != NULL);
}

bool MessageQueue::push(const Packet& pkt) {
    if (!handle) return false;
    return (xQueueSend(handle, &pkt.rawPacket, 0) == pdTRUE);
}

bool MessageQueue::pushFromISR(const Packet& pkt, BaseType_t *higherPriorityTaskWoken) {
    if (!handle) return false;
    return (xQueueSendFromISR(handle, &pkt.rawPacket, higherPriorityTaskWoken) == pdTRUE);
}

bool MessageQueue::pop(Packet& pkt, uint32_t waitMs) {
    if (!handle) return false;
    return (xQueueReceive(handle, &pkt.rawPacket, pdMS_TO_TICKS(waitMs)) == pdTRUE);
}

bool MessageQueue::isEmpty() const {
    if (!handle) return true;
    return (uxQueueMessagesWaiting(handle) == 0);
}

void MessageQueue::clear() {
    if (handle) {
        xQueueReset(handle);
    }
}