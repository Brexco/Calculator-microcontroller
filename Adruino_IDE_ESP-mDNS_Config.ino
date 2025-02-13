/**
* @file ESP8266_Calculator_Server.cpp
* @brief Einfache Webserver-Anwendung zur Berechnung mathematischer Ausdrücke auf einem ESP8266.
* @author Jan Wygoda
* @date 2025-02-12
* @version 01.00.04
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
* @brief Verarbeitung einer mathematischen Berechnung.
* @details Diese Funktion empfängt einen mathematischen Ausdruck als HTTP-Parameter,
*          führt die Berechnung durch und sendet das Ergebnis als Antwort.
*/
void handleCalculate() {
  digitalWrite(iLedProcessing, HIGH); // Verarbeitung starten (LED an)

  if (server.hasArg("expression")) {
    String sExpression = server.arg("expression");
    
    // Operator und Zahlen ermitteln
    char op = 0;
    int opIndex = -1;

    if ((opIndex = sExpression.indexOf('+')) > 0) op = '+';
    else if ((opIndex = sExpression.indexOf('-')) > 0) op = '-';
    else if ((opIndex = sExpression.indexOf('*')) > 0) op = '*';
    else if ((opIndex = sExpression.indexOf('/')) > 0) op = '/';

    if (op != 0) {
      int num1 = sExpression.substring(0, opIndex).toInt();
      int num2 = sExpression.substring(opIndex + 1).toInt();
      float result;

      switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/': 
          if (num2 != 0) {
            result = (float)num1 / num2; 
          } else {
            server.send(400, "text/plain", "Division by 0 not allowed");
            digitalWrite(iLedProcessing, LOW);
            return;
          }
          break;
        default:
          server.send(400, "text/plain", "Invalid operator");
          digitalWrite(iLedProcessing, LOW);
          return;
      }

      server.send(200, "text/plain", String(result));
    } else {
      server.send(400, "text/plain", "Invalid expression. Supported operators: +, -, *, /");
    }
  } else {
    server.send(400, "text/plain", "No expression given");
  }

  digitalWrite(iLedProcessing, LOW); // Verarbeitung beenden (LED aus)
}

/**
* @brief Einrichtung der Hardware und Netzwerkverbindung.
*/
void setup() {
  Serial.begin(115200);

  pinMode(iLedReset, OUTPUT);
  pinMode(iLedServer, OUTPUT);
  pinMode(iLedProcessing, OUTPUT);

  digitalWrite(iLedReset, HIGH);
  digitalWrite(iLedServer, LOW);
  digitalWrite(iLedProcessing, LOW);

  WiFi.begin(cSsid, cPassword);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  if (MDNS.begin("calculator")) {
    Serial.println("mDNS responder started");
  }

  server.on("/calculate", handleCalculate);
  server.begin();
  Serial.println("HTTP-Server gestartet");

  MDNS.addService("http", "tcp", 80);
  digitalWrite(iLedServer, HIGH);
}

/**
* @brief Hauptprogrammschleife zur Verarbeitung von HTTP-Anfragen.
*/
void loop() {
  server.handleClient();
  MDNS.update();
}
