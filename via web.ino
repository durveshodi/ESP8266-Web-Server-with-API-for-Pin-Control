#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

ESP8266WebServer server(80);

int pwmFrequency = 20000; // Default PWM frequency is set to 20 kHz

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>NodeMCU API Functions</h1>";
  html += "<h2>Digital Pin Control</h2>";
  html += "<h3>Read Digital Pin:</h3>";
  html += "<form method=\"POST\" action=\"/api\">";
  html += "Pin: <input type=\"text\" name=\"pin\"><br>";
  html += "<input type=\"hidden\" name=\"action\" value=\"read\">";
  html += "<input type=\"submit\" value=\"Read\">";
  html += "</form>";

  html += "<h3>Write Digital Pin:</h3>";
  html += "<form method=\"POST\" action=\"/api\">";
  html += "Pin: <input type=\"text\" name=\"pin\"><br>";
  html += "Value: <input type=\"number\" name=\"value\"><br>";
  html += "<input type=\"hidden\" name=\"action\" value=\"write\">";
  html += "<input type=\"submit\" value=\"Write\">";
  html += "</form>";

  html += "<h2>Analog Pin Control</h2>";
  html += "<h3>Read Analog Pin:</h3>";
  html += "<form method=\"POST\" action=\"/api\">";
  html += "Pin: <input type=\"text\" name=\"pin\"><br>";
  html += "<input type=\"hidden\" name=\"action\" value=\"read\">";
  html += "<input type=\"submit\" value=\"Read\">";
  html += "</form>";

  html += "<h3>Write Analog Pin:</h3>";
  html += "<form method=\"POST\" action=\"/api\">";
  html += "Pin: <input type=\"text\" name=\"pin\"><br>";
  html += "Value: <input type=\"number\" name=\"value\"><br>";
  html += "<input type=\"hidden\" name=\"action\" value=\"write\">";
  html += "<input type=\"submit\" value=\"Write\">";
  html += "</form>";

  html += "<h2>PWM Control (D1, D2, D3, D4, and D8)</h2>";
  html += "<h3>Generate PWM Signal:</h3>";
  html += "<form method=\"POST\" action=\"/api\">";
  html += "Pin: <input type=\"text\" name=\"pin\"><br>";
  html += "Value: <input type=\"number\" name=\"value\"><br>";
  html += "<input type=\"hidden\" name=\"action\" value=\"pwm\">";
  html += "<input type=\"submit\" value=\"Generate\">";
  html += "</form>";

  html += "<h3>Enable High-Frequency PWM:</h3>";
  html += "<form method=\"POST\" action=\"/api\">";
  html += "Pin: <input type=\"text\" name=\"pin\"><br>";
  html += "Frequency: <input type=\"number\" name=\"frequency\"><br>";
  html += "<input type=\"hidden\" name=\"action\" value=\"enable_high_frequency\">";
  html += "<input type=\"submit\" value=\"Enable\">";
  html += "</form>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleAPI() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");

    // Parse JSON request
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    // Extract action, pin, value, and frequency from JSON
    String action = doc["action"];
    String pinName = doc["pin"];
    int value = doc["value"];
    int frequency = doc["frequency"];

    // Map pin name to pin number
    int pin = -1;
    if (pinName == "D0") {
      pin = D0;
    } else if (pinName == "D1") {
      pin = D1;
    } else if (pinName == "D2") {
      pin = D2;
    } else if (pinName == "D3") {
      pin = D3;
    } else if (pinName == "D4") {
      pin = D4;
    } else if (pinName == "D5") {
      pin = D5;
    } else if (pinName == "D6") {
      pin = D6;
    } else if (pinName == "D7") {
      pin = D7;
    } else if (pinName == "D8") {
      pin = D8;
    } else if (pinName == "A0") {
      pin = A0;
    } else {
      server.send(400, "text/plain", "Invalid pin");
      return;
    }

    // Perform pin control action
    if (action == "read") {
      if (pin >= 0 && pin <= 16) {
        int pinValue = digitalRead(pin);
        server.send(200, "text/plain", String(pinValue));
      } else if (pin == A0) {
        int pinValue = analogRead(A0);
        server.send(200, "text/plain", String(pinValue));
      }
    } else if (action == "write") {
      if (pin >= 0 && pin <= 16) {
        if (value == 0 || value == 1) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, value);
          server.send(200, "text/plain", "Digital pin control successful");
        } else {
          server.send(400, "text/plain", "Invalid value");
        }
      } else if (pin == A0) {
        if (value >= 0 && value <= 1023) {
          analogWrite(A0, value);
          server.send(200, "text/plain", "Analog pin control successful");
        } else {
          server.send(400, "text/plain", "Invalid value");
        }
      }
    } else if (action == "pwm") {
      if (pin >= 0 && pin <= 16) {
        if (value >= 0 && value <= 1023) {
          analogWrite(pin, value);
          server.send(200, "text/plain", "PWM signal generated successfully");
        } else {
          server.send(400, "text/plain", "Invalid value");
        }
      } else {
        server.send(400, "text/plain", "PWM not supported on the specified pin");
      }
    } else if (action == "enable_high_frequency") {
      if (pin >= 0 && pin <= 16) {
        if (pin == D1 || pin == D2 || pin == D3 || pin == D4 || pin == D8) {
          if (frequency > 0 && frequency <= 1000000) {
            pwmFrequency = frequency;
            analogWriteFreq(pwmFrequency);
            server.send(200, "text/plain", "High-frequency PWM enabled successfully");
          } else {
            server.send(400, "text/plain", "Invalid frequency value");
          }
        } else {
          server.send(400, "text/plain", "High-frequency PWM not supported on the specified pin");
        }
      } else {
        server.send(400, "text/plain", "Invalid pin");
      }
    }
  } else {
    server.send(400, "text/plain", "No data received");
  }
}

void setup() {
  WiFiManager wifiManager;

  wifiManager.autoConnect("NodeMCU");

  Serial.begin(115200);
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); // Root page handler
  server.on("/api", HTTP_POST, handleAPI); // API endpoint for pin control

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
