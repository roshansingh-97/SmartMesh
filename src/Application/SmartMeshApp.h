#ifndef SMART_MESH_APP_H
#define SMART_MESH_APP_H

#include "../UI/DisplayManager.h"
#include "../UI/UIManager.h"
#include "../UI/Screens/HomeScreen.h"
#include "../UI/Screens/ComposeScreen.h"
#include "../UI/Screens/InboxScreen.h"
#include "../UI/Screens/SentScreen.h"
#include "../UI/Screens/SettingsScreen.h"
#include "../UI/Screens/AboutScreen.h"
#include "../UI/Screens/ContactsScreen.h"
#include "../Radio/MeshRadio.h"
#include "../Radio/BLERadio.h"

class SmartMeshApp {
private:
    DisplayManager displayManager;
    UIManager uiManager;

    HomeScreen homeScreen;
    ComposeScreen composeScreen;
    InboxScreen inboxScreen;
    SentScreen sentScreen;
    SettingsScreen settingsScreen;
    AboutScreen aboutScreen;
    ContactsScreen contactsScreen;

    MeshRadio radio;
    BLERadio bleRadio;

public:
    SmartMeshApp();
    void begin();
    void update();
    void handleKey(char key);

    // Directly send a keypad-composed message to the connected phone over BLE.
    bool sendComposedMessage(const char* text);

    // Send outgoing messages over ESP-NOW.
    bool sendMessage(uint16_t targetNodeId, const char* text, uint8_t mode = MODE_2_SCT_TO_SCT);
};

#endif // SMART_MESH_APP_H