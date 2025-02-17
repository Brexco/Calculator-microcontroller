/*
* Author:  Jan Wygoda
* Version: 01.00.05
* Date:    12.02.2025
* File:    main.cpp
* @brief   Nimmt die Benutzereingaben für eine mathematische Berechnung entgegen.
* @remarks Sendet den Term zur Berechnung an uC, entgegennimmt das Ergebnis und loggt die Schritte.
*/

//////////////////////////////
//      System Includes     //
//////////////////////////////
#include <iostream>
#include <windows.h>
#include <string>
#include <curl/curl.h>
#include <fstream>
#include <limits>

/**
* @brief   Callback-Funktion zum Schreiben von Daten in einen String.
* @remarks Diese Funktion wird von cURL verwendet, um empfangene Daten in einen String zu speichern.
* @param   contents Zeiger auf den Datenblock, der geschrieben werden soll.
* @param   size Größe eines einzelnen Datenblocks.
* @param   nmemb Anzahl der Datenblöcke.
* @param   output Zeiger auf den String, in den die Daten geschrieben werden.
* @return  Die Gesamtzahl der geschriebenen Bytes.
*/
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
* @brief   Kodiert mathematische Operatoren für eine URL.
* @remarks Ersetzt bestimmte Operatoren durch ihre URL-kodierten Entsprechungen.
* @param   cOperation Der mathematische Operator, der kodiert werden soll.
* @return  Der URL-kodierte Operator als String.
*/
std::string encodeOperator(const char cOperation)
{
    if (cOperation == '+') return "%2B";
    if (cOperation == '*') return "%2A";
    if (cOperation == '/') return "%2F";
    return std::string(1, cOperation);
}

/**
* @brief   Ruft Webinhalt von einer angegebenen URL ab.
* @remarks Verwendet cURL, um eine HTTP-Anfrage an die angegebene URL zu senden.
* @param   sUrl Die URL, von der der Inhalt abgerufen werden soll.
* @return  Der Inhalt der Webseite als String.
*/
std::string fetchWebContent(const std::string& sUrl)
{
    CURL* curl;
    CURLcode res;
    std::string sResponse;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sResponse);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Error retrieving the website: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return sResponse;
}

/**
* @brief   Speichert eine Berechnung und das Ergebnis in einer Datei.
* @remarks Öffnet eine Datei und hängt die Berechnung an das Ende an.
* @param   sCalculation Der mathematische Ausdruck als String.
* @param   sResult Das Ergebnis der Berechnung.
*/
void saveToFile(const std::string& sCalculation, const std::string& sResult)
{
    std::ofstream file("calculations.txt", std::ios::app);
    if (file) {
        file << sCalculation << " = " << sResult << std::endl;
        file.close();
    }
    else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

/**
* @brief   Führt eine mathematische Berechnung durch.
* @remarks Fragt Benutzereingaben ab, sendet eine Anfrage an die URL und speichert das Ergebnis.
*/
void performCalculation()
{
    double dNum1, dNum2;
    char cOperation;

    system("cls");
    std::cout << "Simple natural number calculator\nAddition (+) Subtraction (-) Multiplication (*) Division (/)" << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;

    std::cout << "Enter the first number: ";
    if (!(std::cin >> dNum1)) {
        std::cerr << "Error: Invalid input! Please enter a valid number." << std::endl;
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        return;
    }

    std::cout << "Enter the arithmetic operation (+, -, *, /): ";
    std::cin >> cOperation;
    if (cOperation != '+' && cOperation != '-' && cOperation != '*' && cOperation != '/') {
        std::cerr << "Error: Invalid operator! Please enter one of the following: +, -, *, /" << std::endl;
        return;
    }

    std::cout << "Enter the second number: ";
    if (!(std::cin >> dNum2)) {
        std::cerr << "Error: Invalid input! Please enter a valid number." << std::endl;
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        return;
    }

    std::string encodedOp = encodeOperator(cOperation);
    std::string sUrl = "http://calculator.local/calculate?expression=" + std::to_string(dNum1) + encodedOp + std::to_string(dNum2);

    std::cout << "Connecting to URL: " << sUrl << std::endl;
    std::string sPageContent = fetchWebContent(sUrl);
    std::cout << "Server response:\n" << sPageContent << std::endl;

    saveToFile(std::to_string(dNum1) + " " + cOperation + " " + std::to_string(dNum2), sPageContent);
}

/**
* @brief   Startet das Hauptprogramm.
* @remarks Führt Berechnungen in einer Schleife aus, bis der Benutzer das Programm beendet.
* @return  0, wenn das Programm erfolgreich beendet wurde (Wenn was anderes als y/Y eingegeben wurde).
*/
int main() {
    while (true) {
        performCalculation();
        std::cout << "\nAnother calculation? (Y)es press any other key to leave: ";
        char cWahl;
        std::cin >> cWahl;
        if (cWahl != 'y' && cWahl != 'Y') {
            break;
        }
    }
    return 0;
}
