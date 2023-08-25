/**
 * Arduino Nano 33 IoT - ThingSpeak Temperature Logger
 * 
 * This program reads temperature data from a DHT22 sensor connected to an Arduino Nano 33 IoT,and then sends this data to a ThingSpeak channel over WiFi.
 * 
 * Componenets Used:
 * - Arduino Nano 33 IoT board
 * - DHT22 temperature and humidity sensor
 * 
 * Libraries Used:
 * - WiFiNINA library
 * - ThingSpeak library
 * - DHT library by Adafruit
 * 
 * Made By: Gaganveer Singh (2210994783)
 * Date:25 August 2023
 */

#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// DHT sensor settings
#define DHTPIN 2  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


// WiFi Details
char ssid[] = "Diamond";          // Wifi Name
char password[] = "diamond@580";  // wifi Password

// ThingSpeak Channel ID and Write API Key
unsigned long channelID = 2251816;
const char *writeAPIKey = "J47XOJRIPKHAKJB8";


WiFiClient client;  // Wificlient instance for Wifi connections

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Checking for Wifi module availability
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true) // Wait till module available
      ;
  }

  // connect to Wifi Network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    delay(1000);
  }
  Serial.println("WiFi connection established.");
}

void loop() {
  // Temperature read from DHT22
  float tempCelsius = dht.readTemperature();

  // If reading valid sending data to ThingSpeak
  if (!isnan(tempCelsius)) {
    Serial.print("Temp: ");
    Serial.println(tempCelsius);

    ThingSpeak.setField(1, tempCelsius); // Setting field 1 of channel to temperature data
    
    // Update ThingSpeak channel
    int response = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (response == 200) { // If data sent succesfully
      Serial.println("Temperature data sent suucessfully to ThingSpeak.");
    }
  } else { // Error if there's and issue 
    Serial.println("Sensor read error!");
  }

  // Waiting for 30 seconds before sending data
  delay(30000);
}