#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

// WiFi-Konfiguration
const char* ssid = "USER_SSID";
const char* password = "USER_SSID_PASSWORD";

// LED-Pins
const int ledReset = D5;    // Blaue LED: Reset
const int ledServer = D6;   // Grüne LED: Server online
const int ledProcessing = D7; // Gelbe LED: Verarbeitung

ESP8266WebServer server(80); // HTTP-Server auf Port 80

// Funktion für Berechnungen
void handleCalculate() {
  digitalWrite(ledProcessing, HIGH); // Verarbeitung starten (LED an)

  if (server.hasArg("expression")) {
    String expression = server.arg("expression");
    
    // Operator und Zahlen ermitteln
    char op = 0;
    int opIndex = -1;

    if ((opIndex = expression.indexOf('+')) > 0) op = '+';
    else if ((opIndex = expression.indexOf('-')) > 0) op = '-';
    else if ((opIndex = expression.indexOf('*')) > 0) op = '*';
    else if ((opIndex = expression.indexOf('/')) > 0) op = '/';

    if (op != 0) {
      int num1 = expression.substring(0, opIndex).toInt();
      int num2 = expression.substring(opIndex + 1).toInt();
      float result;

      switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
          if (num2 != 0) {
            result = (float)num1 / num2; 
          } else {
            server.send(400, "text/plain", "Division durch 0 nicht erlaubt");
            digitalWrite(ledProcessing, LOW); // Verarbeitung beenden (LED aus)
            return;
          }
          break;
      }

      server.send(200, "text/plain", String(result));
    } else {
      server.send(400, "text/plain", "Ungültiger Ausdruck. Unterstützte Operatoren: +, -, *, /");
    }
  } else {
    server.send(400, "text/plain", "Kein Ausdruck angegeben");
  }

  digitalWrite(ledProcessing, LOW); // Verarbeitung beenden (LED aus)
}

void setup() {
  // Seriellen Monitor starten
  Serial.begin(115200);

  // LED-Pins als Ausgang definieren
  pinMode(ledReset, OUTPUT);
  pinMode(ledServer, OUTPUT);
  pinMode(ledProcessing, OUTPUT);

  // LEDs initialisieren
  digitalWrite(ledReset, HIGH); // Blaue LED ein: Stromversorgung
  digitalWrite(ledServer, LOW); // Grüne LED aus: Server noch nicht online
  digitalWrite(ledProcessing, LOW); // Gelbe LED aus: Keine Verarbeitung

  // WiFi verbinden
  WiFi.begin(ssid, password);

  Serial.print("Verbindung zu WiFi wird hergestellt");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbunden mit WiFi");

  // mDNS starten
  if (MDNS.begin("calculator")) { // Hostname: calculator.local
    Serial.println("mDNS responder gestartet");
  }

  // Webserver konfigurieren
  server.on("/calculate", handleCalculate);
  server.begin();
  Serial.println("HTTP-Server gestartet");

  // Dienst über mDNS bekannt machen
  MDNS.addService("http", "tcp", 80);

  digitalWrite(ledServer, HIGH); // Grüne LED ein: Server online
}

void loop() {
  server.handleClient(); // HTTP-Anfragen bearbeiten
  MDNS.update();         // mDNS aktualisieren
}
