#include <Arduino.h>
#include "SmartMonitor.h"

// --------------------------
// Define pins
// --------------------------
#define MQ2_PIN 34
#define BUZZER_PIN 12
#define DHT_PIN 14
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
const long updateInterval = 1000; // 1 second

void setup() {
  Serial.begin(115200);
  monitor.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    monitor.readTemperature();
    monitor.readHumidity();
    monitor.readSmoke();
    monitor.readColor();
    monitor.printReadings();
  }
}
