#include "MeshRadio.h"

QueueHandle_t MeshRadio::xQueueOutgoing = NULL;
QueueHandle_t MeshRadio::xQueueIncoming = NULL;

const uint8_t broadcastMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Send Status Callback
void MeshRadio::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Optional: Log send status
}

// Receive Callback (Compatible with both ESP-IDF v4 and v5)
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
void MeshRadio::onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
#else
void MeshRadio::onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
#endif
    if (len == sizeof(SmartMeshPacket)) {
        SmartMeshPacket packet;
        memcpy(&packet, incomingData, sizeof(packet));

        if (packet.receiverID == NODE_ID || packet.receiverID == 0xFF) {
            if (xQueueIncoming != NULL) {
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                xQueueSendFromISR(xQueueIncoming, &packet, &xHigherPriorityTaskWoken);
                if (xHigherPriorityTaskWoken) {
                    portYIELD_FROM_ISR();
                }
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
}

bool MeshRadio::sendPacket(SmartMeshPacket* packet) {
    esp_err_t result = esp_now_send(broadcastMAC, (uint8_t*)packet, sizeof(SmartMeshPacket));
    return (result == ESP_OK);
}

bool MeshRadio::waitForACK(uint16_t packetID, uint32_t timeoutMs) {
    uint32_t start = millis();
    SmartMeshPacket rxPacket;

    while (millis() - start < timeoutMs) {
        // Poll queue for incoming ACK packets
        if (xQueueReceive(xQueueIncoming, &rxPacket, pdMS_TO_TICKS(10)) == pdTRUE) {
            // Check if packet is an ACK and matches the expected packet ID
            if (rxPacket.packetType == PACKET_TYPE_ACK && rxPacket.msgID == packetID) {
                return true;
            }
            // If it's a regular message, put it back or process it separately
        }
    }
    return false; // Timed out waiting for ACK
}

bool MeshRadio::sendWithRetry(SmartMeshPacket& packet, uint8_t maxRetries) {
    packet.crc = calculateCRC16((uint8_t*)&packet, sizeof(SmartMeshPacket) - sizeof(uint16_t));
    
    for (uint8_t attempt = 0; attempt < maxRetries; attempt++) {
        esp_err_t result = esp_now_send(broadcastMAC, (uint8_t*)&packet, sizeof(SmartMeshPacket));
        
        if (result == ESP_OK) {
            if (waitForACK(packet.msgID, 300)) {
                return true;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    return false;
}
