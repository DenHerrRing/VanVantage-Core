# **VanVantage Core**

VanVantage Core ist eine ESP32-basierte IoT-Lösung, die Funktionen wie WLAN-Konfiguration über ein Captive Portal, Over-the-Air (OTA)-Updates und die Verwaltung von WLAN-Verbindungen bietet. Dieses Projekt ist optimiert für Van-Life-Anwendungen, um smarte Steuerung und Konnektivität zu ermöglichen.

---

## **Inhaltsverzeichnis**

1. [Projektbeschreibung](#projektbeschreibung)
2. [Installation und Einrichtung](#installation-und-einrichtung)
3. [Module](#module)
   - [Captive Portal](#captive-portal)
   - [OTA Updater](#ota-updater)
   - [WiFi Manager](#wifi-manager)

---

## **Projektbeschreibung**

VanVantage Core ist ein flexibles Framework, das den ESP32 in ein zentrales Steuermodul für IoT-Devices verwandelt. Es bietet:

- **Einfache WLAN-Konfiguration:** Über ein Captive Portal können Nutzer WLAN-Zugangsdaten bequem eingeben.
- **Drahtlose Firmware-Updates (OTA):** Halten Sie Ihr System stets aktuell, ohne den ESP32 physisch anzuschließen.
- **Modulares Design:** Ermöglicht einfache Anpassungen und Erweiterungen.

Das Projekt eignet sich besonders für smarte Anwendungen im mobilen Einsatz wie Vans, Wohnmobile oder Off-Grid-Lösungen.

---

## **Installation und Einrichtung**

### **1. Voraussetzungen**

- **Hardware:**
  - ESP32-Board
  - Micro-USB-Kabel
- **Software:**
  - [VS Code](https://code.visualstudio.com/) mit [PlatformIO](https://platformio.org/)
  - Git

### **2. Projekt klonen**

Klonen Sie das Repository mit Git:
```bash
git clone https://github.com/yourusername/VanVantage-Core.git
cd VanVantage-Core
```

### **3. Abhängigkeiten installieren**

PlatformIO installiert Abhängigkeiten automatisch basierend auf der `platformio.ini`. Öffnen Sie das Projekt in VS Code und lassen Sie die Abhängigkeiten laden.

### **4. Projekt bauen und hochladen**

1. **Build:** Klicken Sie auf das Häkchen-Icon in der PlatformIO-Sidebar oder führen Sie den Befehl aus:
   ```bash
   pio run
   ```
2. **Hochladen:** Stellen Sie sicher, dass das ESP32-Board verbunden ist, und klicken Sie auf den Upload-Pfeil oder verwenden Sie:
   ```bash
   pio run --target upload
   ```

### **5. Serielle Konsole öffnen**

Überprüfen Sie die Ausgabe in der seriellen Konsole:
```bash
pio device monitor
```

---

## **Module**

### **Captive Portal**

Das Captive Portal startet, wenn keine WLAN-Zugangsdaten vorhanden sind oder das Gerät nicht mit einem Netzwerk verbunden werden kann. Es erstellt einen Access Point (AP), über den Benutzer die Zugangsdaten eingeben können.

#### **Funktionen:**

- Erzeugt einen WLAN-Access Point mit einem Webserver.
- Zeigt eine HTML-Oberfläche zur Eingabe der WLAN-Daten.
- Speichert die Zugangsdaten in den `Preferences` und startet den ESP32 neu.

#### **Codebeispiel:**
```cpp
CaptivePortal portal(wifiManager);
portal.start();
```

---

### **OTA Updater**

Der OTA Updater ermöglicht drahtlose Firmware-Updates. Er überprüft eine `version.json` auf einem Webserver, um festzustellen, ob eine neue Firmware-Version verfügbar ist, und lädt die entsprechende `firmware.bin` herunter.

#### **Funktionen:**

- Initialisierung des OTA-Services.
- Prüfung und Anwendung neuer Firmware-Versionen.
- Speichert die aktuelle Firmware-Version in den `Preferences`.

#### **Codebeispiel:**
```cpp
otaUpdater.setHostName("esp32-my-device");
otaUpdater.begin();
otaUpdater.checkForUpdates("http://dein-server.de/firmware/esp32/version.json");
```

---

### **WiFi Manager**

Der WiFi Manager verwaltet die Verbindung zu einem WLAN. Er kann Zugangsdaten speichern und laden, sich automatisch mit dem Netzwerk verbinden und den Verbindungsstatus prüfen.

#### **Funktionen:**

- Speichert und lädt WLAN-Zugangsdaten mit `Preferences`.
- Verbindet sich automatisch mit dem gespeicherten Netzwerk.
- Bietet Methoden zur Prüfung des Verbindungsstatus.

#### **Codebeispiel:**
```cpp
wifiManager.loadWiFiCredentials();
if (wifiManager.credentialsAreLoaded()) {
    wifiManager.connectToWiFi();
}
```

---

Mit VanVantage Core haben Sie ein leistungsfähiges und flexibles Framework, das smarte IoT-Anwendungen ermöglicht. Viel Erfolg bei Ihrem Projekt!

