#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <Preferences.h>

class WiFiManager {
public:
    WiFiManager();
    void saveWiFiCredentials(const char* ssid, const char* password);
    void loadWiFiCredentials();
    bool credentialsAreLoaded();
    void connectToWiFi();
    bool isConnected();
    void resetStorage();

private:
    Preferences preferences;
    String ssid;
    String password;
};

#endif // WIFIMANAGER_H
