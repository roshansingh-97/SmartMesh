#ifndef MESH_RADIO_H
#define MESH_RADIO_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "Config.h"
#include "Utils/CRC16.h"

class MeshRadio {
public:
    static QueueHandle_t xQueueOutgoing;
    static QueueHandle_t xQueueIncoming;
    static QueueHandle_t xQueueACK; // Dedicated ACK queue to avoid swallowing data packets

    void begin();
    bool sendPacket(SmartMeshPacket* packet);
    bool sendWithRetry(SmartMeshPacket& packet, uint8_t maxRetries = 3);
    bool waitForACK(uint16_t packetID, uint32_t timeoutMs);
    void sendACK(uint16_t msgID, uint8_t targetNode);

    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    static void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len);
#else
    static void onDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
#endif
};

#endif