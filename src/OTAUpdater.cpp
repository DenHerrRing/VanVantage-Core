#include "OTAUpdater.h"
#include <Arduino.h>
#include <ArduinoJson.h>

OTAUpdater::OTAUpdater() {
    loadHostName();
    loadCurrentVersion();
}

void OTAUpdater::begin() {
    ArduinoOTA.setHostname(hostname.c_str());

    ArduinoOTA.onStart([]() {
        Serial.println("OTA Update startet...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA Update abgeschlossen.");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Fortschritt: %u%%\r", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Fehler [%u]\n", error);
    });

    ArduinoOTA.begin();
    Serial.println("OTA bereit. Hostname: " + hostname);
    Serial.println("Aktuelle Firmware-Version: " + currentVersion);
}

void OTAUpdater::handle() {
    ArduinoOTA.handle();
}

void OTAUpdater::setHostName(const char* newHostname) {
    hostname = String(newHostname);
    saveHostName(newHostname);
}

String OTAUpdater::getHostName() {
    return hostname;
}

void OTAUpdater::loadHostName() {
    preferences.begin("ota", true);
    hostname = preferences.getString("hostname", "VanVantage-Core");
    preferences.end();
    Serial.printf("Geladener Hostname: %s\n", hostname.c_str());
}

void OTAUpdater::saveHostName(const char* newHostname) {
    preferences.begin("ota", false);
    preferences.putString("hostname", newHostname);
    preferences.end();
    Serial.printf("Hostname gespeichert: %s\n", newHostname);
}

void OTAUpdater::loadCurrentVersion() {
    preferences.begin("ota", true);
    currentVersion = preferences.getString("version", "1.0.0"); // Standardwert
    preferences.end();
    Serial.printf("Geladene Firmware-Version: %s\n", currentVersion.c_str());
}

void OTAUpdater::saveCurrentVersion(const char* version) {
    preferences.begin("ota", false);
    preferences.putString("version", version);
    preferences.end();
    Serial.printf("Firmware-Version gespeichert: %s\n", version);
}

String OTAUpdater::getCurrentVersion() {
    return currentVersion;
}

void OTAUpdater::setCurrentVersion(const char* version) {
    currentVersion = String(version);
    saveCurrentVersion(version);
}

void OTAUpdater::checkForUpdates(const char* versionUrl) {
    HTTPClient http;
    http.begin(versionUrl);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();

        // Erstelle ein statisches JSON-Dokument mit fester Größe
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
            Serial.println("Fehler beim Parsen der JSON-Daten.");
            return;
        }

        String serverVersion = doc["version"];
        String firmwareUrl = doc["url"];

        Serial.printf("Server-Version: %s, Aktuelle Version: %s\n", serverVersion.c_str(), currentVersion.c_str());

        if (serverVersion > currentVersion) {
            Serial.println("Neue Firmware verfügbar. Starte Update...");
            if (downloadAndUpdate(firmwareUrl)) {
                setCurrentVersion(serverVersion.c_str());
                Serial.println("Update erfolgreich. Neustart...");
                ESP.restart();
            } else {
                Serial.println("Update fehlgeschlagen.");
            }
        } else {
            Serial.println("Firmware ist aktuell.");
        }
    } else {
        Serial.printf("Fehler beim Abrufen der Version: HTTP %d\n", httpCode);
    }

    http.end();
}

bool OTAUpdater::downloadAndUpdate(const String& url) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        int contentLength = http.getSize();
        bool canBegin = Update.begin(contentLength);

        if (canBegin) {
            WiFiClient* stream = http.getStreamPtr();
            size_t written = Update.writeStream(*stream);

            if (written == contentLength) {
                Serial.println("Firmware erfolgreich heruntergeladen.");
                return Update.end();
            } else {
                Serial.println("Fehler beim Schreiben der Firmware.");
                return false;
            }
        } else {
            Serial.println("Update konnte nicht gestartet werden.");
        }
    } else {
        Serial.printf("Fehler beim Herunterladen der Firmware: HTTP %d\n", httpCode);
    }

    http.end();
    return false;
}
