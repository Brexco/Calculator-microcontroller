/*
* Author:  Jan Wygoda
* Version: 01.00.04
* Date:    12.02.2025
* File:    main.cpp
* Remark:  Nimmt die Benutzereingaben für eine mathematische Berechnung entgegen und sendet den Term zur Berechnung an uC.
           Entgegennahme und Anzeige des Ergebnisses. Die einzelnen Schritte werden im Logging festgehalten.
*/

//////////////////////////////
//      System Includes     //
//////////////////////////////
#include <iostream>
#include <windows.h>
#include <string>
#include <curl/curl.h>
#include <fstream>

using namespace std;

/**
* @brief  Callback-Funktion zum Schreiben von Daten in einen String
* @remarks  Diese Funktion wird von cURL verwendet, um empfangene Daten in einen String zu schreiben.
*           Sie wird bei HTTP-Anfragen mit cURL als Schreib-Callback registriert.
* @param contents  Zeiger auf den Datenblock, der geschrieben werden soll.
* @param size  Größe eines einzelnen Datenblocks.
* @param nmemb  Anzahl der Datenblöcke.
* @param output  Zeiger auf den String, in den die Daten geschrieben werden.
* @return  Die Gesamtzahl der geschriebenen Bytes.
*/
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
* @brief  Kodiert mathematische Operatoren für eine URL.
* @param cOperation  Der mathematische Operator, der kodiert werden soll.
* @return  Der URL-kodierte Operator als String.
*/
string encodeOperator(const char cOperation) {
    if (cOperation == '+') return "%2B";
    if (cOperation == '*') return "%2A";
    if (cOperation == '/') return "%2F";
    return string(1, cOperation);
}

/**
* @brief  Ruft Webinhalt von einer angegebenen URL ab.
* @param sUrl  Die URL, von der der Inhalt abgerufen werden soll.
* @return  Der Inhalt der Webseite als String.
*/
string fetchWebContent(const string& sUrl) {
    CURL* curl;
    CURLcode res;
    string sResponse;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sResponse);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            cerr << "Error retrieving the website: " << curl_easy_strerror(res) << endl;
        }
    }
    return sResponse;
}

/**
* @brief  Speichert eine Berechnung und das Ergebnis in einer Datei.
* @param sCalculation  Der mathematische Ausdruck als String.
* @param sResult  Das Ergebnis der Berechnung.
*/
void saveToFile(const string& sCalculation, const string& sResult) {
    ofstream file("calculations.txt", ios::app);
    if (file) {
        file << sCalculation << " = " << sResult << endl;
        file.close();
    } else {
        cerr << "Error opening file for writing." << endl;
    }
}

int main() {
    double dNum1, dNum2;
    char cOperation;

    while (true) {
        system("cls");
        cout << "Simple natural number calculator\nAddition (+) Subtraction (-) Multiplication (*) Division (/)" << endl;
        cout << "----------------------------------------------------------------" << endl;

        cout << "Enter the first number: ";
        cin >> dNum1;
        cout << "Enter the arithmetic operation (+, -, *, /): ";
        cin >> cOperation;
        cout << "Enter the second number: ";
        cin >> dNum2;

        string encodedOp = encodeOperator(cOperation);
        string sUrl = "http://calculator.local/calculate?expression=" + to_string(dNum1) + encodedOp + to_string(dNum2);

        cout << "Connecting to URL: " << sUrl << endl;

        string sPageContent = fetchWebContent(sUrl);
        cout << "Server response:\n" << sPageContent << endl;

        saveToFile(to_string(dNum1) + " " + cOperation + " " + to_string(dNum2), sPageContent);

        cout << "\nAnother calculation? (Y)es press any other key to leave: ";
        char cWahl;
        cin >> cWahl;
        if (cWahl != 'y' && cWahl != 'Y') {
            break;
        }
    }
    return 0;
}
