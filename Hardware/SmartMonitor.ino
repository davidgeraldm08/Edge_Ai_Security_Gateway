#include <Arduino.h>
#include "SmartMonitor.h"
#include <WiFi.h>
#include <HTTPClient.h>


// --------------------------
// Define pins
// --------------------------
#define MQ2_PIN 32
#define BUZZER_PIN 13
#define DHT_PIN 33
#define DHT_TYPE DHT22
#define S0 4
#define S1 5
#define S2 18
#define S3 19
#define OUT_PIN 23

// Create SmartMonitor object
SmartMonitor monitor(MQ2_PIN, BUZZER_PIN, DHT_PIN, DHT_TYPE, S0, S1, S2, S3, OUT_PIN);

// Timing variables for non-blocking updates
unsigned long previousMillis = 0;
unsigned long previousSavedMillis = 0;
const long saveInterval = 60000;
const long updateInterval = 1000; // 1 second


const char* ssid = "Marlon_Hotspot";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid,password);

  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  monitor.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long saveMillis = millis();
  
  if(saveMillis - previousSavedMillis >= saveInterval){
    previousSavedMillis = saveMillis; 
    Serial.println(("Data Saving......."));
    monitor.readTemperature();
    monitor.readSmoke();
    monitor.readColor();
    monitor.printReadings();
    monitor.saveData();

  }

  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    monitor.readTemperature();
    monitor.readSmoke();
    monitor.readColor();
    monitor.printReadings();
    
  }
  

}