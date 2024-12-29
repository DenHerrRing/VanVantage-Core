#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager::WiFiManager() : ssid(""), password("") {}

void WiFiManager::saveWiFiCredentials(const char* ssid, const char* password) {
    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
    Serial.println("WLAN-Daten gespeichert!");
}

void WiFiManager::loadWiFiCredentials() {
    preferences.begin("wifi", true);
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
    preferences.end();

    if (ssid.isEmpty() || password.isEmpty()) {
        Serial.println("Keine gespeicherten WLAN-Daten gefunden.");
    } else {
        Serial.printf("Geladene WLAN-Daten: SSID: %s, Passwort: %s\n", ssid.c_str(), password.c_str());
    }
}

bool WiFiManager::credentialsAreLoaded() {
    return !(ssid.isEmpty() || password.isEmpty());
}

void WiFiManager::connectToWiFi() {
    if (!credentialsAreLoaded()) {
        Serial.println("Keine WLAN-Daten geladen. Verbindung nicht möglich.");
        return;
    }

    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.printf("Verbinde mit SSID: %s...\n", ssid.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nVerbunden mit WLAN!");
    Serial.printf("IP-Adresse: %s\n", WiFi.localIP().toString().c_str());
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::resetStorage() {
    // Alle gespeicherten Daten in den Preferences löschen
    preferences.begin("wifi", false); // Namespace öffnen
    preferences.clear(); // Alle Daten im Namespace löschen
    preferences.end(); // Namespace schließen

    // Neustart des ESP32
    Serial.println("Neustart...");
    delay(2000);
    ESP.restart();
}