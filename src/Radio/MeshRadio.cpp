#include "MeshRadio.h"

QueueHandle_t MeshRadio::xQueueOutgoing = NULL;
QueueHandle_t MeshRadio::xQueueIncoming = NULL;
QueueHandle_t MeshRadio::xQueueACK = NULL;

const uint8_t broadcastMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void MeshRadio::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Optional debug logging
}

#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
void MeshRadio::onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
#else
void MeshRadio::onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
#endif
    if (len == sizeof(SmartMeshPacket)) {
        SmartMeshPacket packet;
        memcpy(&packet, incomingData, sizeof(packet));

        // Validate CRC before processing
        uint16_t calculatedCRC = calculateCRC16((uint8_t*)&packet, sizeof(SmartMeshPacket) - sizeof(uint16_t));
        if (packet.crc != calculatedCRC) {
            return; // Corrupted packet, drop
        }

        if (packet.receiverID == NODE_ID || packet.receiverID == 0xFF) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;

            // Route ACKs to xQueueACK, and regular data to xQueueIncoming
            if (packet.packetType == PACKET_TYPE_ACK) {
                if (xQueueACK != NULL) {
                    xQueueSendFromISR(xQueueACK, &packet, &xHigherPriorityTaskWoken);
                }
            } else {
                if (xQueueIncoming != NULL) {
                    xQueueSendFromISR(xQueueIncoming, &packet, &xHigherPriorityTaskWoken);
                }
            }

            if (xHigherPriorityTaskWoken) {
                portYIELD_FROM_ISR();
            }
        }
    }
}

void MeshRadio::begin() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    xQueueOutgoing = xQueueCreate(10, sizeof(SmartMeshPacket));
    xQueueIncoming = xQueueCreate(10, sizeof(SmartMeshPacket));
    xQueueACK      = xQueueCreate(5, sizeof(SmartMeshPacket));
}

bool MeshRadio::sendPacket(SmartMeshPacket* packet) {
    packet->crc = calculateCRC16((uint8_t*)packet, sizeof(SmartMeshPacket) - sizeof(uint16_t));
    esp_err_t result = esp_now_send(broadcastMAC, (uint8_t*)packet, sizeof(SmartMeshPacket));
    return (result == ESP_OK);
}

void MeshRadio::sendACK(uint16_t msgID, uint8_t targetNode) {
    SmartMeshPacket ackPacket;
    ackPacket.senderID = NODE_ID;
    ackPacket.receiverID = targetNode;
    ackPacket.msgID = msgID;
    ackPacket.packetType = PACKET_TYPE_ACK;
    sendPacket(&ackPacket);
}

bool MeshRadio::waitForACK(uint16_t packetID, uint32_t timeoutMs) {
    uint32_t start = millis();
    SmartMeshPacket rxPacket;

    while (millis() - start < timeoutMs) {
        // Poll dedicated ACK queue
        if (xQueueReceive(xQueueACK, &rxPacket, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (rxPacket.msgID == packetID) {
                return true;
            }
        }
    }
    return false; // Timeout
}

bool MeshRadio::sendWithRetry(SmartMeshPacket& packet, uint8_t maxRetries) {
    for (uint8_t attempt = 0; attempt < maxRetries; attempt++) {
        if (sendPacket(&packet)) {
            if (waitForACK(packet.msgID, 300)) {
                return true;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    return false;
}