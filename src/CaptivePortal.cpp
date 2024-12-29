#include "CaptivePortal.h"
#include <Arduino.h>

CaptivePortal::CaptivePortal(WiFiManager& wifiManager) : server(80), wifiManager(wifiManager) {}

void CaptivePortal::start() {
    WiFi.softAP("VanVantage-CaptivePortal"); // AP-Modus starten
    IPAddress IP = WiFi.softAPIP();
    Serial.printf("Access Point gestartet. IP: %s\n", IP.toString().c_str());

    // DNS-Server einrichten
    dnsServer.start(53, "*", IP); // Leitet alle DNS-Anfragen auf die ESP-IP um

    // Webserver-Routen definieren
    server.on("/", std::bind(&CaptivePortal::handleRoot, this));
    server.on("/save", std::bind(&CaptivePortal::handleSave, this));
    server.onNotFound([this]() {
        server.sendHeader("Location", "/", true); // Weiterleitung auf "/"
        server.send(302, "text/plain", "Redirecting...");
    });

    server.begin();
    Serial.println("Webserver gestartet. Captive Portal aktiv.");

    // Hauptloop für DNS- und Webserver
    while (true) {
        dnsServer.processNextRequest();
        server.handleClient();
    }
}

void CaptivePortal::handleRoot() {
    String html = R"rawliteral(
        <!DOCTYPE html>
        <html lang="de">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>VanVantage - Captive Portal</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    background-color: #f0f0f0;
                    margin: 0;
                    padding: 0;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                }
                .container {
                    background: #ffffff;
                    padding: 20px;
                    border-radius: 8px;
                    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
                    max-width: 400px;
                    width: 100%;
                    box-sizing: border-box;
                }
                h1 {
                    text-align: center;
                    font-size: 1.5rem;
                    margin-bottom: 20px;
                }
                form {
                    display: flex;
                    flex-direction: column;
                }
                label {
                    margin-bottom: 5px;
                    font-weight: bold;
                }
                input[type="text"],
                input[type="password"] {
                    padding: 10px;
                    margin-bottom: 15px;
                    border: 1px solid #ccc;
                    border-radius: 4px;
                    font-size: 1rem;
                }
                input[type="submit"] {
                    background-color: #007BFF;
                    color: #fff;
                    padding: 10px;
                    border: none;
                    border-radius: 4px;
                    font-size: 1rem;
                    cursor: pointer;
                }
                input[type="submit"]:hover {
                    background-color: #0056b3;
                }
            </style>
        </head>
        <body>
            <div class="container">
                <h1>WLAN Konfiguration für VanVantage</h1>
                <form action="/save" method="POST">
                    <label for="ssid">SSID:</label>
                    <input type="text" id="ssid" name="ssid" placeholder="Netzwerkname">
                    <label for="password">Passwort:</label>
                    <input type="password" id="password" name="password" placeholder="Netzwerkpasswort">
                    <input type="submit" value="Speichern">
                </form>
            </div>
        </body>
        </html>
    )rawliteral";

    server.send(200, "text/html", html);
}


void CaptivePortal::handleSave() {
    if (server.hasArg("ssid") && server.hasArg("password")) {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        wifiManager.saveWiFiCredentials(ssid.c_str(), password.c_str());
        server.send(200, "text/html", "<h1>WLAN-Daten gespeichert. Neustart...</h1>");
        delay(2000);
        ESP.restart();
    } else {
        server.send(400, "text/html", "<h1>Fehler: SSID und Passwort erforderlich</h1>");
    }
}
