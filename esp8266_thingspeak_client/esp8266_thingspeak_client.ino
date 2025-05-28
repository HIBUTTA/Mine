#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

// Define SoftwareSerial pins for communication with Arduino
SoftwareSerial arduinoSerial(D5, D6); // RX, TX

// WiFi and ThingSpeak credentials
const char* ssid     = "STARLINK_SAT_01";        // Your WiFi SSID
const char* password = "18273645";               // Your WiFi password
const char* apiKey   = "P4SPS175G6Q68AQK";       // Your ThingSpeak Write API Key
const char* server   = "api.thingspeak.com";

// Variables for data
int waterLevel = 0;
int temperature = 0;
int ldrValue = 0;
int boardStatus = 1; // Example board status, change as needed

WiFiClient client;

void setup() {
  Serial.begin(9600);  
  arduinoSerial.begin(9600); // Initialize SoftwareSerial for communication with Arduino
  delay(10);
  Serial.println("ESP8266 Ready");
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Waiting for WiFi connection...");
  }
  
  Serial.println("WiFi connected!");
}

void loop() {
  // Check if data is available from Arduino
  if (arduinoSerial.available()) {
    String receivedData = arduinoSerial.readStringUntil('\n'); // Read data until newline
    receivedData.trim(); // Remove any leading/trailing whitespace
    Serial.println("Received: " + receivedData); // Print received data
    
    // Split concatenated readings
    while (receivedData.length() > 0) {
      int delimiterIndex = receivedData.indexOf("Received: ");
      
      if (delimiterIndex != -1) {
        // Extract one reading
        String singleReading = receivedData.substring(delimiterIndex, receivedData.indexOf("Received: ", delimiterIndex + 1));
        if (singleReading == "") {
          singleReading = receivedData.substring(delimiterIndex);
        }
        
        processReading(singleReading); // Process the extracted reading
        
        // Remove the processed reading from the string
        receivedData = receivedData.substring(delimiterIndex + singleReading.length());
      } else {
        // No more valid readings, exit the loop
        break;
      }
    }
  }

  delay(100); // Add a short delay for stability
}

void processReading(String reading) {
  reading.trim();
  
  if (reading.startsWith("Received: ")) {
    reading = reading.substring(10); // Remove "Received: " prefix
    int firstColon = reading.indexOf(':');
    int secondColon = reading.lastIndexOf(':');

    if (firstColon != -1 && secondColon != -1 && firstColon < secondColon) {
      // Extract values
      waterLevel = reading.substring(0, firstColon).toInt();
      temperature = reading.substring(firstColon + 1, secondColon).toInt();
      ldrValue = reading.substring(secondColon + 1).toInt();

      // Debug print parsed values
      Serial.println("Parsed Data:");
      Serial.println("Water Level: " + String(waterLevel));
      Serial.println("Temperature: " + String(temperature));
      Serial.println("LDR Value: " + String(ldrValue));
      
      // Send data to ThingSpeak
      sendDataToThingSpeak();
    } else {
      Serial.println("Error: Invalid data format in reading.");
    }
  } else {
    Serial.println("Error: Data does not start with 'Received: '.");
  }
}

void sendDataToThingSpeak() {
  // Construct the POST data string
  String postData = "api_key=" + String(apiKey) +
                    "&field1=" + String(waterLevel) +
                    "&field2=" + String(temperature) +
                    "&field3=" + String(ldrValue) +
                    "&field4=" + String(boardStatus);
  
  // Connect to ThingSpeak and send the data
  if (client.connect(server, 80)) {
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);

    Serial.println("Data Sent to ThingSpeak:");
    Serial.println(postData);
    
    // Wait for the server's response
    while (client.connected() || client.available()) {
      if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.println("Response: " + response);
      }
    }
    client.stop();
  } 
  else {
    Serial.println("Connection to ThingSpeak failed.");
  }
  
  // Wait 20 seconds (ThingSpeak free accounts allow updates every 15 sec)
  delay(20000);
}
