/**
* @author Jan Wygoda
* @version 01.00.05
* @date 17.02.2025
* @file ESP8266_Calculator_Server.cpp
* @brief Einfache Webserver-Anwendung zur Berechnung mathematischer Ausdrücke auf einem ESP8266.
*/

//////////////////////////////
//      System Includes     //
//////////////////////////////
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

// WiFi-Konfiguration
const char* cSsid = "USER_SSID";
const char* cPassword = "USER_SSID_PASSWORD";

// LED-Pins
const int iLedReset = D5;    ///< Blaue LED: Reset
const int iLedServer = D6;   ///< Grüne LED: Server online
const int iLedProcessing = D7; ///< Gelbe LED: Verarbeitung

ESP8266WebServer server(80); // HTTP-Server auf Port 80

/**
* @brief Einrichtung der Hardware und Netzwerkverbindung.
*/
void setup() {
  Serial.begin(115200);

  setupPins();
  connectToWiFi();
  startMDNS();
  startServer();
}

/**
* @brief Konfiguriert die Pins.
*/
void setupPins() {
  pinMode(iLedReset, OUTPUT);
  pinMode(iLedServer, OUTPUT);
  pinMode(iLedProcessing, OUTPUT);

  digitalWrite(iLedReset, HIGH);
  digitalWrite(iLedServer, LOW);
  digitalWrite(iLedProcessing, LOW);
}

/**
* @brief Stellt die Verbindung zum WiFi her.
*/
void connectToWiFi() {
  WiFi.begin(cSsid, cPassword);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

/**
* @brief Startet mDNS.
*/
void startMDNS() {
  if (MDNS.begin("calculator")) {
    Serial.println("mDNS responder started");
  }

  MDNS.addService("http", "tcp", 80);
}

/**
* @brief Startet den HTTP-Server und registriert die Endpunkte.
*/
void startServer() {
  server.on("/calculate", handleCalculate);
  server.begin();
  Serial.println("HTTP-Server gestartet");
  digitalWrite(iLedServer, HIGH);
}

/**
* @brief Verarbeitung einer mathematischen Berechnung.
* @details Diese Funktion empfängt einen mathematischen Ausdruck als HTTP-Parameter,
*          führt die Berechnung durch und sendet das Ergebnis als Antwort.
*/
void handleCalculate() {
  digitalWrite(iLedProcessing, HIGH); // Verarbeitung starten (LED an)

  if (server.hasArg("expression")) {
    String expression = server.arg("expression");
    float result = performCalculation(expression);
    
    if (isnan(result)) {
      server.send(400, "text/plain", "Invalid expression or operator.");
    } else {
      server.send(200, "text/plain", String(result));
    }
  } else {
    server.send(400, "text/plain", "No expression given");
  }

  digitalWrite(iLedProcessing, LOW); // Verarbeitung beenden (LED aus)
}

/**
* @brief Führt die mathematische Berechnung durch.
* @param expression Der mathematische Ausdruck als String.
* @return Das Ergebnis der Berechnung oder NaN, falls ungültig.
*/
float performCalculation(const String& expression) {
  char op = 0;
  int opIndex = findOperator(expression, op);

  if (opIndex == -1) {
    return NAN; // Ungültiger Ausdruck
  }

  int num1 = expression.substring(0, opIndex).toInt();
  int num2 = expression.substring(opIndex + 1).toInt();

  switch (op) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/': 
      if (num2 != 0) {
        return (float)num1 / num2;
      } else {
        return NAN; // Division durch 0
      }
    default: return NAN; // Ungültiger Operator
  }
}

/**
* @brief Findet den Operator im Ausdruck.
* @param expression Der mathematische Ausdruck als String.
* @param op Der Operator, der gefunden wird.
* @return Der Index des Operators oder -1, wenn kein gültiger Operator gefunden wurde.
*/
int findOperator(const String& expression, char& op) {
  int opIndex = expression.indexOf('+');
  if (opIndex > 0) { op = '+'; return opIndex; }
  
  opIndex = expression.indexOf('-');
  if (opIndex > 0) { op = '-'; return opIndex; }
  
  opIndex = expression.indexOf('*');
  if (opIndex > 0) { op = '*'; return opIndex; }
  
  opIndex = expression.indexOf('/');
  if (opIndex > 0) { op = '/'; return opIndex; }
  
  return -1; // Kein Operator gefunden
}

/**
* @brief Hauptprogrammschleife zur Verarbeitung von HTTP-Anfragen.
*/
void loop() {
  server.handleClient();
  MDNS.update();
}
