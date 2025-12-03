#ifndef SMARTMONITOR_H
#define SMARTMONITOR_H

#include <Arduino.h>

// ---------------- MQ2 Smoke Sensor ----------------
class SmokeSensor {
private:
    int analogPin;
    int buzzerPin;
    int threshold;
    bool connected;

public:
    SmokeSensor(int aPin, int bPin, bool isConnected = true, int thresh = 700);
    void begin();
    bool detectSmoke();
    void setThreshold(int t);          // Adjust sensitivity
    bool isConnected() const { return connected; }
};

// ---------------- Temperature Sensor ----------------
class TemperatureSensor {
private:
    float temperatureOffset;
    float temperature;
    bool connected;

public:
    TemperatureSensor(bool isConnected = true);
    void begin();
    void setOffset(float offset);
    void readTemperature();
    float getTemperature() const { return temperature; }
    bool isConnected() const { return connected; }
};

// ---------------- TCS3200 Color Sensor ----------------
class ColorSensor {
private:
    int S0, S1, S2, S3, OUT_PIN, LED_PIN;
    int red, green, blue;
    int readColorFrequency(int s2State, int s3State);

public:
    ColorSensor(int s0, int s1, int s2, int s3, int outPin, int ledPin);
    void begin();
    void ledOn();
    void ledOff();
    void readColor();
    String detectColor();
    int getRed() const { return red; }
    int getGreen() const { return green; }
    int getBlue() const { return blue; }
};

// ---------------- SmartMonitor Class ----------------
class SmartMonitor {
private:
    SmokeSensor smokeSensor;
    TemperatureSensor tempSensor;
    ColorSensor colorSensor;

public:
    SmartMonitor(int mq2Pin, int buzzerPin, bool mq2Connected,
                 bool tempConnected,
                 int tcsS0, int tcsS1, int tcsS2, int tcsS3, int tcsOut, int tcsLed);

    void begin();
    void readSensors();
    void printReadings();
    void setTemperatureOffset(float offset);

    // New public wrapper for MQ2 threshold
    void setSmokeThreshold(int t);
};

#endif
