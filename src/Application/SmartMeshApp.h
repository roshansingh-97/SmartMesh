#ifndef SMART_MESH_APP_H
#define SMART_MESH_APP_H

#include "../UI/UIManager.h"
#include "../UI/DisplayManager.h"
#include "../UI/Screens/HomeScreen.h"
#include "../UI/Screens/ComposeScreen.h"
#include "../UI/Screens/InboxScreen.h"
#include "../UI/Screens/SentScreen.h"
#include "../UI/Screens/SettingsScreen.h"

class SmartMeshApp {
private:
    UIManager uiManager;
    DisplayManager displayManager;

    // Screen Instances
    HomeScreen homeScreen;
    ComposeScreen composeScreen;
    InboxScreen inboxScreen;
    SentScreen sentScreen;
    SettingsScreen settingsScreen;

public:
    SmartMeshApp() = default;

    void begin();
    void update();
    void handleKeyPress(char key); // <--- Add this declaration
};

#endif // SMART_MESH_APP_H