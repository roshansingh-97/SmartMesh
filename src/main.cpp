#include <Arduino.h>
#include <cstring>
#include "Config.h"
#include "DisplayManager.h"
#include "KeypadHandler.h"
#include "MeshRadio.h"

DisplayManager display;
KeypadHandler  keypad;
MeshRadio      meshRadio;

TaskHandle_t TaskUIHandle;
TaskHandle_t TaskNetworkHandle;

// --- CORE 1: UI & KEYPAD TASK ---
void TaskUI(void *pvParameters) {
    SystemState currentState = STATE_HOME_MENU;
    String currentBuffer = "";
    uint8_t menuIndex = 0;

    for (;;) {
        char key = keypad.scanKeypad();
        
        if (key != '\0') {
            bool isFinalized = false;
            char typedChar = keypad.processMultiTap(key, isFinalized);

            if (currentState == STATE_HOME_MENU) {
                if (key == 'A' && menuIndex > 0) menuIndex--;        // Up
                if (key == 'B' && menuIndex < 4) menuIndex++;        // Down
                if (key == 'D' || key == '#') {                      // Select
                    if (menuIndex == 0) currentState = STATE_COMPOSE;
                }
            } 
            else if (currentState == STATE_COMPOSE) {
                if (isFinalized && typedChar != '\0') {
                    if (typedChar == '*') {
                        if (currentBuffer.length() > 0) {
                            currentBuffer.remove(currentBuffer.length() - 1);
                        }
                    } else if (typedChar == '#') { // SEND MESSAGE
                        SmartMeshPacket packet;
                        packet.msgID = (uint16_t)(millis() & 0xFFFF);
                        packet.senderID = NODE_ID;
                        packet.receiverID = 0x02; // Target Terminal
                        packet.packetType = PACKET_TYPE_DATA;
                        
                        memset(packet.payload, 0, sizeof(packet.payload));
                        strncpy(packet.payload, currentBuffer.c_str(), sizeof(packet.payload) - 1);

                        if (MeshRadio::xQueueOutgoing != NULL) {
                            xQueueSend(MeshRadio::xQueueOutgoing, &packet, portMAX_DELAY);
                        }
                        
                        currentBuffer = "";
                        currentState = STATE_HOME_MENU;
                    } else if (typedChar == 'C') { // CANCEL
                        currentBuffer = "";
                        currentState = STATE_HOME_MENU;
                    } else {
                        currentBuffer += typedChar;
                    }
                }
            }
        }

        // Render current state display
        if (currentState == STATE_HOME_MENU) {
            display.renderMenu(menuIndex);
        } else if (currentState == STATE_COMPOSE) {
            display.renderComposeScreen(currentBuffer);
        }

        vTaskDelay(pdMS_TO_TICKS(20)); // Yield CPU
    }
}

// --- CORE 0: NETWORK & RADIO TASK ---
void TaskNetwork(void *pvParameters) {
    SmartMeshPacket txPacket;
    SmartMeshPacket rxPacket;

    for (;;) {
        // 1. Process Outgoing Queue
        if (MeshRadio::xQueueOutgoing != NULL && 
            xQueueReceive(MeshRadio::xQueueOutgoing, &txPacket, pdMS_TO_TICKS(10))) {
            meshRadio.sendWithRetry(txPacket, 3);
        }

        // 2. Process Incoming Queue
        if (MeshRadio::xQueueIncoming != NULL && 
            xQueueReceive(MeshRadio::xQueueIncoming, &rxPacket, pdMS_TO_TICKS(10))) {
            // Process incoming packets (save to message history or send ACK)
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void setup() {
    Serial.begin(115200);

    display.begin();
    keypad.begin();
    meshRadio.begin();

    // Spawn dual-core FreeRTOS tasks
    xTaskCreatePinnedToCore(TaskUI, "TaskUI", 4096, NULL, 2, &TaskUIHandle, 1);
    xTaskCreatePinnedToCore(TaskNetwork, "TaskNetwork", 4096, NULL, 3, &TaskNetworkHandle, 0);
}

void loop() {
    vTaskDelete(NULL); // Free main loop task context
}