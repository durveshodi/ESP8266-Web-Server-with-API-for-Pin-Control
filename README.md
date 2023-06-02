# ESP8266 Web Server with API for Pin Control

This is an ESP8266-based Arduino project that creates a web server with an API for controlling digital and analog pins. It allows you to perform actions such as reading pin values, writing digital values, generating PWM signals, and enabling high-frequency PWM.

## Hardware Requirements
- ESP8266-based board (e.g., NodeMCU)
- LEDs, resistors, and/or other components for testing (optional)

## Software Requirements
- Arduino IDE
- ESP8266WiFi library
- ESP8266WebServer library
- ArduinoJson library
- WiFiManager library

## Installation

1. Open the Arduino IDE.
2. Install the required libraries by navigating to **Sketch -> Include Library -> Manage Libraries**. Search for each library and click **Install**.
3. Copy the provided code into a new Arduino sketch.

## Usage

1. Connect your ESP8266-based board to your computer using a USB cable.
2. Select the appropriate board and port from the **Tools** menu in the Arduino IDE.
3. Compile and upload the sketch to your board.
4. Open the Serial Monitor in the Arduino IDE to monitor the board's IP address.
5. Connect your computer or mobile device to the same Wi-Fi network as the board.
6. Open a web browser and enter the board's IP address in the address bar.
7. You should see a web page with the WiFiManager configuration portal. Connect to the network named "NodeMCU" and enter your Wi-Fi credentials.
8. Once connected, the board's IP address will be displayed in the Serial Monitor.
9. Use the provided API to control the pins:

### API Endpoints

- **POST /api** - This endpoint accepts JSON data to control the pins.

  Example JSON request:
  ```
  {
    "action": "read",
    "pin": "D1"
  }
  ```

  The `action` field can have the following values:
  - `"read"`: Reads the value of the specified pin.
  - `"write"`: Writes a digital value (0 or 1) to the specified pin.
  - `"pwm"`: Generates a PWM signal on the specified pin with the provided analog value.
  - `"enable_high_frequency"`: Enables high-frequency PWM on the specified pin with the provided frequency.

  The `pin` field can have the following values:
  - `"D0"`, `"D1"`, `"D2"`, `"D3"`, `"D4"`, `"D5"`, `"D6"`, `"D7"`, `"D8"`: Digital pins.
  - `"A0"`: Analog pin.

  The `value` field is required for `"write"` and `"pwm"` actions. It should be a digital value (0 or 1) for `"write"` action or an analog value (0-1023) for `"pwm"` action.

  The `frequency` field is required for `"enable_high_frequency"` action and should be a frequency value in Hz (1-1000000).

  The server will respond with the result of the action or an error message.

## Conclusion

This project demonstrates how to create a web server with an API for controlling pins on an ESP8266-based board. By using the provided API endpoints, you can read pin values, write digital values, generate PWM signals, and enable high-frequency PWM. Feel free to modify the code and add more functionality to suit your needs.
