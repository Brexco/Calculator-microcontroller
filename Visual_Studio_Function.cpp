#include <iostream>
#include <windows.h>
#include <string>
#include <curl/curl.h>
#include <fstream>

// Callback-Funktion für cURL, um den Inhalt zu speichern
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Funktion zum Ersetzen des Operators
std::string encodeOperator(char operation) {
    if (operation == '+') return "%2B";
    if (operation == '*') return "%2A";
    if (operation == '/') return "%2F";
    return std::string(1, operation);
}

// Funktion zum Abrufen des Webseiteninhalts
std::string fetchWebContent(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Error retrieving the website: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return response;
}

// Funktion zur Speicherung der Berechnung in eine Datei
void saveToFile(const std::string& calculation, const std::string& result) {
    std::ofstream file("calculations.txt", std::ios::app);
    if (file.is_open()) {
        file << calculation << " = " << result << std::endl;
        file.close();
    }
    else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}

int main() {
    double num1, num2;
    char operation;

    while (true) {
        system("cls");
        std::cout << "Simple natural number calculator\nAddition (+) Subtraction (-) Multiplication (*) Division (/)" << std::endl;
        std::cout << "----------------------------------------------------------------" << std::endl;

        std::cout << "Enter the first number: ";
        std::cin >> num1;
        std::cout << "Enter the arithmetic operation (+, -, *, /): ";
        std::cin >> operation;
        std::cout << "Enter the second number:";
        std::cin >> num2;

        // URL erstellen
        std::string encodedOp = encodeOperator(operation);
        std::string sUrl = "http://calculator.local/calculate?expression=" + std::to_string(num1) + encodedOp + std::to_string(num2);

        std::cout << "Connecting to URL: " << sUrl << std::endl;

        // Serverantwort abrufen
        std::string pageContent = fetchWebContent(sUrl);
        std::cout << "Server response:\n" << pageContent << std::endl;

        // Berechnung in Datei speichern
        saveToFile(std::to_string(num1) + " " + operation + " " + std::to_string(num2), pageContent);

        // Wiederholen oder Beenden
        std::cout << "\nAnother calculation? (Y)es: ";
        char wahl;
        std::cin >> wahl;
        if (wahl != 'y' && wahl != 'Y') {
            break;
        }
    }
    return 0;
}
