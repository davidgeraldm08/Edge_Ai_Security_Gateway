#include "SmartMonitor.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include "time.h"
#include <ArduinoJson.h>
#include <DHT.h>

// Constructor

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 8 * 3600;   // GMT+8 (Philippines)
const int daylightOffset_sec = 0;
const int smokeThreshold = 1600;
DHT dht(33, DHT22);


SmartMonitor::SmartMonitor(int mq2Pin, int buzzerPin, int dhtPin, int dhtType,
                           int tcsS0, int tcsS1, int tcsS2, int tcsS3, int tcsOut)
  : mq2Pin(mq2Pin), buzzerPin(buzzerPin), dhtPin(dhtPin), dhtType(dhtType),
    S0(tcsS0), S1(tcsS1), S2(tcsS2), S3(tcsS3), OUT_PIN(tcsOut), dht(dhtPin, dhtType)
{
}

// Initialization
void SmartMonitor::begin() {
  
  dht.begin();
  

  pinMode(mq2Pin, INPUT);
  pinMode(buzzerPin, OUTPUT);


  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_PIN, INPUT);


  // TCS3200 scaling 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("\nTime synced!");

  Serial.println("SmartMonitor Initialized");
}

// -------------------------
// Read Sensors
// -------------------------

void SmartMonitor::readTemperature() {
  temperature = dht.readTemperature();
  delay(2000);
}

void SmartMonitor::readSmoke() {
  smokeValue = analogRead(mq2Pin);
  smokeDetected = (smokeValue > smokeThreshold);

  // Activate buzzer if smoke detected
  digitalWrite(buzzerPin, smokeDetected ? HIGH : LOW);
  delay(2000);
}

// Private function to read TCS3200 frequency
int SmartMonitor::readColorFrequency(int s2State, int s3State) {
  digitalWrite(S2, s2State);
  digitalWrite(S3, s3State);
  delay(2000);
  return pulseIn(OUT_PIN, LOW);
}

void SmartMonitor::readColor() {
  red = readColorFrequency(LOW, LOW);
  green = readColorFrequency(HIGH, HIGH);
  blue = readColorFrequency(LOW, HIGH);
}

// Determine dominant color or natural light
String SmartMonitor::detectColor() {
  int minColorThreshold = 500; // adjust based on ambient light

  if (red < minColorThreshold && green < minColorThreshold && blue < minColorThreshold) {
    return "Natural Light";
  } else if (red < green && red < blue){
     return "Red Light";
     }else if (green < red && green < blue){
       return "Green Light";
       }else if (blue < red && blue < green){ 
        return "Blue Light";
        }

  return "Mixed Light";
}

// -------------------------
// Print Readings
// -------------------------
void SmartMonitor::printReadings() {
 
  Serial.println("=====================================");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Smoke Value: "); Serial.println(smokeValue);
  Serial.println(smokeDetected ? "⚠️  SMOKE DETECTED!" : "No Smoke Detected");

  Serial.print("Color Sensor: "); Serial.println(detectColor());
  Serial.print("R: "); Serial.print(red);
  Serial.print(" | G: "); Serial.print(green);
  Serial.print(" | B: "); Serial.println(blue);
  Serial.println("=====================================\n");

   if (WiFi.status() == WL_CONNECTED) {

  DynamicJsonDocument doc(200);  
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    String dateTime = String();
    dateTime = dateTime + (timeinfo.tm_year + 1900) + "-" +
                      (timeinfo.tm_mon + 1) + "-" +
                      timeinfo.tm_mday + " " +
                      timeinfo.tm_hour + ":" +
                      timeinfo.tm_min + ":" +
                      timeinfo.tm_sec;
    doc["time_log"] = dateTime;
  }
  HTTPClient http;

    String url = "http://172.20.10.6:5000/api/live/"; 
    http.begin(url);

    // Important JSON header
    http.addHeader("Content-Type", "application/json");

    doc["temp_log"] = temperature;
    doc["gas_log"] = smokeDetected;
    doc["color_log"] = String(red)+","+String(green)+","+String(blue);
    String json;
    serializeJson(doc, json);

    
    int httpCode = http.POST(json);

    Serial.print("HTTP Code: ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("Server Response:");
      Serial.println(response);
    } else {
      Serial.print("POST Error: ");
      Serial.println(httpCode);
    }
 
    Serial.println(millis());

    http.end();
  }
}

void SmartMonitor::saveData() {
  if (WiFi.status() == WL_CONNECTED){
    DynamicJsonDocument doc(200);  
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      String dateTime = String();
      dateTime = dateTime + (timeinfo.tm_year + 1900) + "-" +
                        (timeinfo.tm_mon + 1) + "-" +
                        timeinfo.tm_mday + " " +
                        timeinfo.tm_hour + ":" +
                        timeinfo.tm_min + ":" +
                        timeinfo.tm_sec;
      doc["time_log"] = dateTime;
    }  

    HTTPClient http;
    String url = "http://172.20.10.6:5000/api/data/"; 
    http.begin(url);
    doc["temp_log"] = temperature;
    doc["gas_log"] = smokeDetected;
    doc["color_log"] = String(red)+","+String(green)+","+String(blue);
    String json;
    serializeJson(doc, json);
    
    // Important JSON header
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(json);

      Serial.print("HTTP Code: ");
      Serial.println(httpCode);

      if (httpCode > 0) {
        String response = http.getString();
        Serial.println("Server Response:");
        Serial.println(response);
        Serial.println("DATA SAVED");
      } else {
        Serial.print("POST Error: ");
        Serial.println(httpCode);
      }
  
      Serial.println(millis());

      http.end();
  }

}