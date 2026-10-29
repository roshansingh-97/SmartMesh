#include "MeshRadio.h"

QueueHandle_t MeshRadio::xQueueOutgoing = nullptr;
QueueHandle_t MeshRadio::xQueueIncoming = nullptr;
QueueHandle_t MeshRadio::xQueueACK = nullptr;

const uint8_t broadcastMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void MeshRadio::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Optional debug logging.
}

#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
void MeshRadio::onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
#else
void MeshRadio::onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
#endif
    if (incomingData == nullptr || len != sizeof(SmartMeshPacket)) {
        return;
    }

    SmartMeshPacket packet = {};
    memcpy(&packet, incomingData, sizeof(packet));

    const uint16_t calculatedCRC = calculateCRC16(
        reinterpret_cast<uint8_t*>(&packet),
        sizeof(SmartMeshPacket) - sizeof(uint16_t)
    );
    if (packet.crc != calculatedCRC) {
        return;
    }

    // Node-address broadcast is 0x00FF in this protocol (not 0xFFFF).
    if (packet.receiverID != NODE_ID && packet.receiverID != 0x00FF) {
        return;
    }

    // ESP-NOW receive callbacks execute in the Wi-Fi task, not an ISR.
    // Use xQueueSend() rather than xQueueSendFromISR().
    if (packet.packetType == PACKET_TYPE_ACK) {
        if (xQueueACK != nullptr) {
            xQueueSend(xQueueACK, &packet, 0);
        }
    } else {
        if (xQueueIncoming != nullptr) {
            xQueueSend(xQueueIncoming, &packet, 0);
        }
    }
}

void MeshRadio::begin() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("[ESP-NOW] ERROR: esp_now_init() failed.");
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastMAC, sizeof(broadcastMAC));
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    const esp_err_t peerResult = esp_now_add_peer(&peerInfo);
    if (peerResult != ESP_OK && peerResult != ESP_ERR_ESPNOW_EXIST) {
        Serial.printf("[ESP-NOW] Warning: add broadcast peer failed (%d).\n", peerResult);
    }

    if (xQueueOutgoing == nullptr) xQueueOutgoing = xQueueCreate(10, sizeof(SmartMeshPacket));
    if (xQueueIncoming == nullptr) xQueueIncoming = xQueueCreate(10, sizeof(SmartMeshPacket));
    if (xQueueACK == nullptr) xQueueACK = xQueueCreate(5, sizeof(SmartMeshPacket));
}

bool MeshRadio::sendPacket(SmartMeshPacket* packet) {
    if (packet == nullptr) {
        return false;
    }
    packet->crc = calculateCRC16(
        reinterpret_cast<uint8_t*>(packet),
        sizeof(SmartMeshPacket) - sizeof(uint16_t)
    );
    return esp_now_send(broadcastMAC, reinterpret_cast<uint8_t*>(packet), sizeof(SmartMeshPacket)) == ESP_OK;
}

void MeshRadio::sendACK(uint16_t msgID, uint16_t targetNode) {
    SmartMeshPacket ackPacket = {};
    ackPacket.mode = MODE_2_SCT_TO_SCT;
    ackPacket.packetType = PACKET_TYPE_ACK;
    ackPacket.senderID = NODE_ID;
    ackPacket.receiverID = targetNode;
    ackPacket.msgID = msgID;
    sendPacket(&ackPacket);
}

bool MeshRadio::waitForACK(uint16_t packetID, uint32_t timeoutMs) {
    if (xQueueACK == nullptr) {
        return false;
    }

    const uint32_t start = millis();
    SmartMeshPacket rxPacket = {};
    while (millis() - start < timeoutMs) {
        if (xQueueReceive(xQueueACK, &rxPacket, pdMS_TO_TICKS(10)) == pdTRUE &&
            rxPacket.msgID == packetID) {
            return true;
        }
    }
    return false;
}

bool MeshRadio::sendWithRetry(SmartMeshPacket& packet, uint8_t maxRetries) {
    for (uint8_t attempt = 0; attempt < maxRetries; attempt++) {
        if (sendPacket(&packet) && waitForACK(packet.msgID, 300)) {
            return true;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    return false;
}
