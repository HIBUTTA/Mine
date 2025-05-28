# Automatic Aquarium Project

This repository contains the code for an automatic aquarium monitoring and control system. It uses an Arduino to read sensor data and an ESP8266 to send this data to ThingSpeak for remote monitoring.

## Project Structure

- **`arduino_sensor_controller/`**: Contains the Arduino sketch (`arduino_sensor_controller.ino`).
  - This code is responsible for:
    - Reading data from a water level sensor, a temperature sensor (e.g., LM35), and a Light Dependent Resistor (LDR).
    - Controlling a water pump based on the water level.
    - Sending the sensor data to the ESP8266 via SoftwareSerial.

- **`esp8266_thingspeak_client/`**: Contains the ESP8266 sketch (`esp8266_thingspeak_client.ino`).
  - This code is responsible for:
    - Connecting to a WiFi network.
    - Receiving sensor data from the Arduino via SoftwareSerial.
    - Parsing the received data.
    - Sending the parsed data (water level, temperature, LDR value, and board status) to a ThingSpeak channel.

- **`esp8266_thingspeak_test/`**: Contains a test sketch for the ESP8266 (`esp8266_thingspeak_test.ino`).
  - This code is used for testing the WiFi connection and ThingSpeak data upload functionality.
  - It simulates sensor data randomly and sends it to ThingSpeak. This is useful for verifying the ESP8266 and ThingSpeak setup independently of the Arduino.

## Hardware Requirements (Example)

**Arduino Side:**
- Arduino board (e.g., Uno, Nano)
- Water level sensor
- Temperature sensor (e.g., LM35)
- LDR sensor
- Water pump
- Relay module for pump control (if needed)
- Connecting wires

**ESP8266 Side:**
- ESP8266 module (e.g., NodeMCU, Wemos D1 Mini)
- Connecting wires

**Communication:**
- Logic level shifter (if connecting 5V Arduino to 3.3V ESP8266 directly for SoftwareSerial)

## Software Setup

1.  **Arduino IDE**: Ensure you have the Arduino IDE installed.
2.  **ESP8266 Core for Arduino IDE**: If you haven't already, add ESP8266 board support to your Arduino IDE.
    - Go to `File > Preferences` and add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to the "Additional Boards Manager URLs".
    - Go to `Tools > Board > Boards Manager...`, search for "esp8266" and install the package by "ESP8266 Community".
3.  **Libraries**:
    - `SoftwareSerial.h`: Usually included with the Arduino IDE.
    - `ESP8266WiFi.h`: Installed with the ESP8266 core.
4.  **ThingSpeak Account**:
    - Create an account on [ThingSpeak.com](https://thingspeak.com/).
    - Create a new channel.
    - Note down your Channel ID and Write API Key.

## Configuration

**1. `arduino_sensor_controller.ino`:**
   - Adjust `WATER_LEVEL_PIN`, `TEMP_SENSOR_PIN`, `LDR_PIN`, and `PUMP_PIN` according to your wiring.
   - Modify the `espSerial` pins if you use different pins for SoftwareSerial communication with the ESP8266.
   - Adjust the pump control logic (`if (waterLevel < 500)`) as needed.

**2. `esp8266_thingspeak_client.ino`:**
   - Update `ssid` and `password` with your WiFi credentials.
   - Update `apiKey` with your ThingSpeak Write API Key.
   - Ensure the `SoftwareSerial arduinoSerial(D5, D6);` pins match the pins you've connected to the Arduino's `espSerial` (TX to RX, RX to TX). D5 and D6 are examples for ESP8266.

**3. `esp8266_thingspeak_test.ino`:**
   - Update `ssid` and `password` with your WiFi credentials.
   - Update `apiKey` with your ThingSpeak Write API Key.

## Wiring (Example ESP8266 and Arduino SoftwareSerial)

- **ESP8266 D5 (RX)** <-> **Arduino Pin 11 (TX of espSerial)**
- **ESP8266 D6 (TX)** <-> **Arduino Pin 10 (RX of espSerial)**
- **Ground (GND)** <-> **Ground (GND)**

**Important:** If your Arduino operates at 5V and your ESP8266 at 3.3V, use a logic level shifter between the serial pins to prevent damage to the ESP8266.

## How to Use

1.  **Program the Arduino**:
    - Open `arduino_sensor_controller/arduino_sensor_controller.ino` in the Arduino IDE.
    - Select your Arduino board and port.
    - Upload the sketch.
2.  **Program the ESP8266**:
    - Open `esp8266_thingspeak_client/esp8266_thingspeak_client.ino` (or `esp8266_thingspeak_test.ino` for testing) in the Arduino IDE.
    - Select your ESP8266 board and port.
    - Upload the sketch.
3.  **Monitor**:
    - Open the Serial Monitor in the Arduino IDE for both the Arduino and ESP8266 (one at a time, or using two IDE instances/serial terminals) to see debug messages.
    - Check your ThingSpeak channel to see the data being uploaded.

## Contributing

Feel free to fork this repository, make improvements, and submit pull requests.
```
