#include "SmartMonitor.h"

// ------------------- MQ2 -------------------
SmokeSensor::SmokeSensor(int aPin, int bPin, bool isConnected, int thresh) {
  analogPin = aPin;
  buzzerPin = bPin;
  connected = isConnected;
  threshold = thresh;
}

void SmokeSensor::begin() {
  if (!connected) return;
  pinMode(analogPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

bool SmokeSensor::detectSmoke() {
  if (!connected) return false;

  int sensorValue = analogRead(analogPin);
  bool detected = sensorValue > threshold;
  digitalWrite(buzzerPin, detected ? HIGH : LOW);
  return detected;
}

// ------------------- Temperature -------------------
TemperatureSensor::TemperatureSensor(bool isConnected) {
  connected = isConnected;
  temperatureOffset = 0.0;
  temperature = 0.0;
}

void TemperatureSensor::begin() {
  if (!connected) return;
}

void TemperatureSensor::setOffset(float offset) {
  temperatureOffset = offset;
}

void TemperatureSensor::readTemperature() {
  if (!connected) {
    temperature = NAN;
    return;
  }

  // Placeholder: replace with actual sensor reading
  temperature = 25.0 - temperatureOffset;
}

// ------------------- TCS3200 -------------------
ColorSensor::ColorSensor(int s0, int s1, int s2, int s3, int outPin, int ledPin) {
  S0 = s0; S1 = s1; S2 = s2; S3 = s3;
  OUT_PIN = outPin;
  LED_PIN = ledPin;
  red = green = blue = 0;
}

void ColorSensor::begin() {
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(OUT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(S0, HIGH); digitalWrite(S1, LOW); // frequency scaling
  digitalWrite(LED_PIN, LOW); // LED OFF
}

void ColorSensor::ledOn() { digitalWrite(LED_PIN, HIGH); }
void ColorSensor::ledOff() { digitalWrite(LED_PIN, LOW); }

int ColorSensor::readColorFrequency(int s2State, int s3State) {
  digitalWrite(S2, s2State);
  digitalWrite(S3, s3State);
  delay(20);
  return pulseIn(OUT_PIN, LOW);
}

void ColorSensor::readColor() {
  ledOff();
  red = readColorFrequency(LOW, LOW);
  green = readColorFrequency(HIGH, HIGH);
  blue = readColorFrequency(LOW, HIGH);
}

// ------------------- Improved Color Detection -------------------
String ColorSensor::detectColor() {
  int minThreshold = 5000;
  int similarityThreshold = 500;

  if (red > minThreshold && green > minThreshold && blue > minThreshold)
    return "Natural Light";

  // Yellow: red + green strong and similar, blue weaker
  if (abs(red - green) < similarityThreshold && red < blue && green < blue)
    return "Yellow Light";

  if (red < green && red < blue) return "Red Light";
  if (green < red && green < blue) return "Green Light";
  if (blue < red && blue < green) return "Blue Light";

  return "Mixed Light";
}

// ------------------- SmartMonitor -------------------
SmartMonitor::SmartMonitor(int mq2Pin, int buzzerPin, bool mq2Connected,
                           bool tempConnected,
                           int tcsS0, int tcsS1, int tcsS2, int tcsS3, int tcsOut, int tcsLed)
  : smokeSensor(mq2Pin, buzzerPin, mq2Connected),
    tempSensor(tempConnected),
    colorSensor(tcsS0, tcsS1, tcsS2, tcsS3, tcsOut, tcsLed) {}

void SmartMonitor::begin() {
  smokeSensor.begin();
  tempSensor.begin();
  colorSensor.begin();
  Serial.println("SmartMonitor Initialized");
}

void SmartMonitor::readSensors() {
  tempSensor.readTemperature();
  smokeSensor.detectSmoke();
  colorSensor.readColor();
}

void SmartMonitor::printReadings() {
  Serial.println("=====================================");

  if (tempSensor.isConnected()) {
    Serial.print("Temperature: "); Serial.print(tempSensor.getTemperature()); Serial.println(" °C");
  } else {
    Serial.println("Temperature Sensor not connected!");
  }

  if (smokeSensor.isConnected()) {
    bool smoke = smokeSensor.detectSmoke();
    Serial.println(smoke ? "⚠️  SMOKE DETECTED!" : "No Smoke Detected");
  } else {
    Serial.println("MQ2 Smoke Sensor not connected!");
  }

  Serial.print("Color Sensor: "); Serial.println(colorSensor.detectColor());
  Serial.print("R: "); Serial.print(colorSensor.getRed());
  Serial.print(" | G: "); Serial.print(colorSensor.getGreen());
  Serial.print(" | B: "); Serial.println(colorSensor.getBlue());
  Serial.println("=====================================\n");
}

void SmartMonitor::setTemperatureOffset(float offset) {
  tempSensor.setOffset(offset);
}
