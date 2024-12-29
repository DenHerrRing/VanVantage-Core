#ifndef CAPTIVEPORTAL_H
#define CAPTIVEPORTAL_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "WiFiManager.h"

class CaptivePortal {
public:
    CaptivePortal(WiFiManager& wifiManager);
    void start();

private:
    WebServer server;
    DNSServer dnsServer;      // DNS-Server für Captive Portal
    WiFiManager& wifiManager; // Referenz auf WiFiManager zur Verwaltung von WLAN-Daten
    void handleRoot();        // HTML-Seite für Zugangsdaten
    void handleSave();        // Verarbeitung der Zugangsdaten
};

#endif // CAPTIVEPORTAL_H
