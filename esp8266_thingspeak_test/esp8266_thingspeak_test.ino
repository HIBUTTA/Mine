#include <ESP8266WiFi.h>

// WiFi and ThingSpeak credentials
const char* ssid     = "STARLINK_SAT_01";        // Your WiFi SSID
const char* password = "18273645";               // Your WiFi password
const char* apiKey   = "P4SPS175G6Q68AQK";       // Your ThingSpeak Write API Key
const char* server   = "api.thingspeak.com";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  delay(10);
  
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Waiting for WiFi connection...");
  }
  
  Serial.println("WiFi connected!");
}

void loop() {
  // Simulate sensor data for testing:
  int waterLevel = random(300, 800);             // Dummy water level value
  int temperature = random(200, 350) / 10;       // Dummy temperature value (20-35°C)
  int ldrValue = random(100, 600);               // Dummy LDR value
  int boardStatus = 1;                           // Board online status
  
  // Construct the POST data string for 4 fields
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
