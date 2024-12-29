#include <Arduino.h>
#include "WiFiManager.h"
#include "CaptivePortal.h"
#include "OTAUpdater.h"

WiFiManager wifiManager;
OTAUpdater otaUpdater;
CaptivePortal captivePortal(wifiManager);

void printBanner() {
    Serial.println();
    Serial.println(R"rawliteral(
 __     __       __     __          _                    ____               
 \ \   / /_ _ _ _\ \   / /_ _ _ __ | |_ __ _  __ _  ___ / ___|___  _ __ ___ 
  \ \ / / _` | '_ \ \ / / _` | '_ \| __/ _` |/ _` |/ _ \ |   / _ \| '__/ _ \
   \ V / (_| | | | \ V / (_| | | | | || (_| | (_| |  __/ |__| (_) | | |  __/
    \_/ \__,_|_| |_|\_/ \__,_|_| |_|\__\__,_|\__, |\___|\____\___/|_|  \___|
                                             |___/                          
    )rawliteral");
    Serial.println();
    Serial.println("VanVantage Core - Powered by ESP32");
    Serial.println("===================================");
}

void setup() {
    Serial.begin(115200);

    // Auskommentieren um Speicher zu löschen.
    // wifiManager.resetStorage();

    // Loader
    printBanner();

    // WLAN-Zugangsdaten laden
    wifiManager.loadWiFiCredentials();

    // Wenn keine Zugangsdaten vorhanden sind, starte das Captive Portal
    if (!wifiManager.credentialsAreLoaded()) {
        Serial.println("Keine gespeicherten WLAN-Daten gefunden. Starte Captive Portal...");
        captivePortal.start();
    }

    // Mit WLAN verbinden
    wifiManager.connectToWiFi();

    if (wifiManager.isConnected()) {
        Serial.println("WLAN-Verbindung hergestellt.");

        // OTA initialisieren
        otaUpdater.begin();

        // Firmware-Update prüfen und ggf. installieren
        otaUpdater.checkForUpdates("https://ota.vanvantage.de/firmware/esp32/version.json");
    } else {
        Serial.println("WLAN-Verbindung fehlgeschlagen. Gerät wird gestoppt.");
        while (true) {
            delay(1000); // Gerät im Stillstand halten
        }
    }
}

void loop() {
    otaUpdater.handle();
}
