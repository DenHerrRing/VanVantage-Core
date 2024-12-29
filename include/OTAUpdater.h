#ifndef OTAUPDATER_H
#define OTAUPDATER_H

#include <ArduinoOTA.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <Update.h>

class OTAUpdater {
public:
    OTAUpdater();
    void begin();
    void handle();
    void setHostName(const char* hostname);
    String getHostName();
    void checkForUpdates(const char* versionUrl); // URL zur `version.json`
    String getCurrentVersion();                  // Aktuelle Version abrufen
    void setCurrentVersion(const char* version); // Aktuelle Version setzen

private:
    Preferences preferences;
    String hostname;
    String currentVersion; // Aktuelle Firmware-Version
    void loadHostName();
    void saveHostName(const char* hostname);
    void loadCurrentVersion();                   // Firmware-Version laden
    void saveCurrentVersion(const char* version); // Firmware-Version speichern
    bool downloadAndUpdate(const String& url);   // Firmware-Download und -Update
};

#endif // OTAUPDATER_H
