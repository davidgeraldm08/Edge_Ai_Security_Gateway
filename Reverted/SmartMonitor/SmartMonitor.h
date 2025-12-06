#ifndef SMARTMONITOR_H
#define SMARTMONITOR_H

#include <Arduino.h>
#include <DHT.h>

class SmartMonitor {
  public:
    // Constructor
    SmartMonitor(int mq2Pin, int buzzerPin, int dhtPin, int dhtType,
                 int tcsS0, int tcsS1, int tcsS2, int tcsS3, int tcsOut);

    // Initialization
    void begin();

    // Sensor readings
    void readTemperature();
    void readHumidity();
    void readSmoke();
    void readColor();

    // Print readings to Serial
    void printReadings();

    // Detect dominant color or natural light
    String detectColor();

    // Accessors
    float getTemperature() const { return temperature; }
    float getHumidity() const { return humidity; }
    int getSmokeValue() const { return smokeValue; }
    bool isSmokeDetected() const { return smokeDetected; }
    int getRed() const { return red; }
    int getGreen() const { return green; }
    int getBlue() const { return blue; }

  private:
    // Pins
    int mq2Pin;
    int buzzerPin;
    int dhtPin;
    int dhtType;
    int S0, S1, S2, S3, OUT_PIN;

    // Sensor objects
    DHT dht;

    // Thresholds
    int smokeThreshold = 1800;

    // Sensor readings
    float temperature;
    float humidity;
    int smokeValue;
    bool smokeDetected;
    String color;
    int red, green, blue;

    // Private function for TCS3200
    int readColorFrequency(int s2State, int s3State);
};

#endif