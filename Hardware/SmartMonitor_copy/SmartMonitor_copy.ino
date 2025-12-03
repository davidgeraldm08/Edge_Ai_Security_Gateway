#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SmartMonitor.h"

// --------------------------
// Define pins and sensor connections
// --------------------------
#define MQ2_PIN 32
#define BUZZER_PIN 13
#define TEMP_CONNECTED true   // set false if DHT/temp sensor not connected
#define MQ2_CONNECTED true    // set false if MQ2 not connected
#define S0 4
#define S1 5
#define S2 18
#define S3 19
#define OUT_PIN 23
#define LED_PIN 12

const char* ssid = "PLDTHOMEFIBR5GKt6B7";
const char* password = "PLDTWIFIcfPB8";

SmartMonitor monitor(MQ2_PIN, BUZZER_PIN, MQ2_CONNECTED, TEMP_CONNECTED,
                     S0, S1, S2, S3, OUT_PIN, LED_PIN);

unsigned long previousMillis = 0;
const long updateInterval = 1000; // 1 second

void setup() {
  Serial.begin(115200);
 

  monitor.begin();
  monitor.setTemperatureOffset(2.0);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    monitor.readSensors();
    monitor.printReadings();
  }
}
