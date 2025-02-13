#include <iostream>
#include <windows.h>
#include <string>
#include <curl/curl.h>
#include <fstream>

using namespace std;

// Callback-Funktion für cURL, um den Inhalt zu speichern
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Funktion zum Ersetzen des Operators
string encodeOperator(char operation) {
    if (operation == '+') return "%2B";
    if (operation == '*') return "%2A";
    if (operation == '/') return "%2F";
    return string(1, operation);
}

// Funktion zum Abrufen des Webseiteninhalts
string fetchWebContent(const string& url) {
    CURL* curl;
    CURLcode res;
    string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            cerr << "Error retrieving the website: " << curl_easy_strerror(res) << endl;
        }
    }
    return response;
}

// Funktion zur Speicherung der Berechnung in eine Datei
void saveToFile(const string& calculation, const string& result) {
    ofstream file("calculations.txt", ios::app);
    if (file.is_open()) {
        file << calculation << " = " << result << endl;
        file.close();
    }
    else {
        cerr << "Error opening file for writing." << endl;
    }
}

int main() {
    double num1, num2;
    char operation;

    while (true) {
        system("cls");
        cout << "Simple natural number calculator\nAddition (+) Subtraction (-) Multiplication (*) Division (/)" << endl;
        cout << "----------------------------------------------------------------" << endl;

        cout << "Enter the first number: ";
        cin >> num1;
        cout << "Enter the arithmetic operation (+, -, *, /): ";
        cin >> operation;
        cout << "Enter the second number:";
        cin >> num2;

        // URL erstellen
        string encodedOp = encodeOperator(operation);
        string sUrl = "http://calculator.local/calculate?expression=" + to_string(num1) + encodedOp + to_string(num2);

        cout << "Connecting to URL: " << sUrl << endl;

        // Serverantwort abrufen
        string pageContent = fetchWebContent(sUrl);
        cout << "Server response:\n" << pageContent << endl;

        // Berechnung in Datei speichern
        saveToFile(to_string(num1) + " " + operation + " " + to_string(num2), pageContent);

        // Wiederholen oder Beenden
        cout << "\nAnother calculation? (Y)es: ";
        char wahl;
        cin >> wahl;
        if (wahl != 'y' && wahl != 'Y') {
            break;
        }
    }
    return 0;
}
