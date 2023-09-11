#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Sahils.iphone";     // Replace with your WiFi credentials
const char* password = "1234567890";
const int relayPin = D1;           // Replace with the GPIO pin your relay is connected to

ESP8266WebServer server(80);

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Print the ESP8266's IP address to the serial monitor
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_OPTIONS, handleOptions);  // Respond to preflight requests
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_GET, handleOn);
  server.on("/off", HTTP_GET, handleOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleOptions() {
  // Handle preflight requests by allowing requests from any origin
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,PUT,POST,DELETE,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, Content-Length, X-Requested-With");
  server.send(204);
}

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP8266 Relay Control</h1>";
  html += "<p><a href='/on'><button onclick='sendRequest(\"/on\")'>Turn On</button></a></p>";
  html += "<p><a href='/off'><button onclick='sendRequest(\"/off\")'>Turn Off</button></a></p>";
  html += "<script>";
  html += "function sendRequest(path) {";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', path, true);";
  html += "xhr.send();";
  html += "}";
  html += "</script>";
  html += "</body></html>";

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(relayPin, HIGH);  // Turn the relay on
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "On request sent");
}

void handleOff() {
  digitalWrite(relayPin, LOW);   // Turn the relay off
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Off request sent");
}

