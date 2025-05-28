#include <SoftwareSerial.h>

// Define sensors and pins
#define WATER_LEVEL_PIN A0
#define TEMP_SENSOR_PIN A1
#define LDR_PIN A2
#define PUMP_PIN 8

SoftwareSerial espSerial(10, 11); // RX, TX for ESP8266

void setup() {
  Serial.begin(9600);        // For debugging
  espSerial.begin(9600);     // For ESP8266 communication
  pinMode(PUMP_PIN, OUTPUT); // Water pump control
}

void loop() {
  // Read sensors
  int waterLevel = analogRead(WATER_LEVEL_PIN);
  int ldrValue = analogRead(LDR_PIN);
  float tempValue = readTemperature();

  // Send data to ESP8266
  espSerial.print("WL:"); espSerial.println(waterLevel);
  espSerial.print("TEMP:"); espSerial.println(tempValue);
  espSerial.print("LDR:"); espSerial.println(ldrValue);

  // Optional: Logic to control water pump
  if (waterLevel < 500) { // Example threshold
    digitalWrite(PUMP_PIN, HIGH);
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }

  delay(2000); // Wait 2 seconds before next reading
}

// Function to read temperature (example for LM35)
float readTemperature() {
  int sensorValue = analogRead(TEMP_SENSOR_PIN);
  return (sensorValue * 5.0 / 1024.0) * 100.0; // Convert to Celsius
}
