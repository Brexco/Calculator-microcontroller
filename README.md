# README
## Overview
This project consists of two separate components:

 - Main Application (main.cpp): A console-based calculator application that interacts with a web server to perform mathematical calculations.
 - ESP8266 Web Server (ESP8266_Calculator_Server.cpp): A simple web server running on an ESP8266 microcontroller, which handles HTTP requests to perform mathematical calculations.

Both components work together to allow users to perform basic arithmetic operations via the web, with the results being logged and displayed.

# Files Description
1. main.cpp
Description:
This C++ application accepts user input for mathematical operations, sends the operation to a web server (hosted on an ESP8266 device), retrieves the result, and logs the calculation in a text file.

Key Features:

 - Takes user input for two numbers and an operator.
 - Sends the input as a mathematical expression to a web server via a GET request.
 - Receives and displays the result from the web server.
 - Saves the operation and result in a file called calculations.txt.
 - Handles basic error checking for invalid inputs.
 - Allows users to repeat calculations until they choose to exit.
   
Dependencies:

 - curl library for HTTP requests.
 - Standard C++ libraries such as <iostream>, <string>, and <fstream>.

Key Functions:

 - encodeOperator: URL-encodes the mathematical operator for the HTTP request.
 - fetchWebContent: Sends an HTTP request to the web server and retrieves the result.
 - saveToFile: Appends the mathematical expression and result to calculations.txt.
 - performCalculation: The main function that coordinates the input collection, HTTP request, and result display.
 - main: The entry point for the program that runs the calculation loop.

# Help 
 1. Hilfe für Setup-Server unter Calculator-microcontroller/Tutorial/setup-server.gif
 2. Hilfe für Setup-client unter Calculator-microcontroller/Tutorial/setup_client.gif
   
# 2. ESP8266_Calculator_Server.cpp
Description:
This program sets up a simple web server on an ESP8266 microcontroller, allowing it to receive mathematical expressions via HTTP requests and return the calculated result.

Key Features:

 - Connects to a Wi-Fi network.
 - Sets up an mDNS responder for easy access to the server by its hostname (calculator.local).
 - Provides an HTTP endpoint (/calculate) that accepts mathematical expressions via query parameters.
 - Processes basic arithmetic expressions and returns the result.
 - Uses LEDs to indicate the server status (online, processing requests, etc.).
   
Dependencies:

 - ESP8266WiFi and ESP8266WebServer libraries for managing the Wi-Fi connection and handling HTTP requests.
 - ESP8266mDNS library for mDNS support.
   
Key Functions:

 - setupPins: Configures the LED pins.
 - connectToWiFi: Connects the ESP8266 to the Wi-Fi network.
 - startMDNS: Starts the mDNS responder so that the ESP8266 can be accessed via calculator.local.
 - startServer: Starts the HTTP server and defines the /calculate endpoint.
 - handleCalculate: Handles incoming HTTP requests, performs the calculation, and sends the result back to the client.
 - performCalculation: Performs the actual mathematical calculation by parsing the expression.
 - findOperator: Finds the operator in the given expression.

# Installation and Setup
1. Main Application (main.cpp)

 1.1 Install curl library in your C++ environment (required for HTTP requests).
 1.2 Compile the main.cpp file using your preferred C++ IDE or compiler.
 1.3 Run the executable.
 
 - The program will prompt for the first number, operator, and second number.
 - It will then send the request to the web server running on the ESP8266 and display the result.
 - Results of each calculation will be logged in calculations.txt.
   
2. ESP8266 Web Server (ESP8266_Calculator_Server.cpp)
   
 2.1 Install the Arduino IDE and the ESP8266 board support.
 2.2 Open the ESP8266_Calculator_Server.cpp file in the Arduino IDE.
 2.3 Modify the cSsid and cPassword variables with your Wi-Fi network's credentials.
 2.4 Select the correct board (ESP8266) and port in the Arduino IDE.
 2.5 Upload the code to the ESP8266.
 2.6 Once the ESP8266 is connected to the Wi-Fi network, it will start an HTTP server accessible via http://calculator.local.
 
# Usage
1. Running the main application:

 - Start the application. It will ask for two numbers and an operator to perform a basic arithmetic operation (e.g., 1 + 1).
 - After entering the input, the result will be displayed.
 - You can continue performing calculations, or type any other key to exit the program.
   
2. Web Server on ESP8266:

 - Access the ESP8266 server via the URL http://calculator.local/calculate?expression=<expression>, where <expression> is a mathematical expression like 1+1.
 - The server will return the result as plain text.
   
# Troubleshooting
Main Application:

 - Ensure that curl is installed and properly configured on your system.
 - If the web server is unreachable, check the ESP8266’s Wi-Fi connection and ensure it is running.
   
ESP8266 Web Server:

 - Ensure the ESP8266 is connected to the Wi-Fi network by checking the serial monitor output.
 - If the ESP8266 is not reachable via calculator.local, ensure mDNS is supported and properly configured on your device.

# Acknowledgments
The project uses the cURL library for making HTTP requests in the C++ application.
The ESP8266 libraries are used to interface with the microcontroller and manage Wi-Fi connectivity and the web server.
