// Include necessary libraries
#include <NewPing.h>
#include <Servo.h>
#include "DHT.h"

// Variable declaration
float humidity;           // Humidity value
float tempCelsius;        // Temperature value in Celsius
float tempFahrenheit;     // Temperature value in Fahrenheit
unsigned int distance;    // Distance value from sonar
int lightSensorValue;     // Light sensor value

// Pin definitions
#define TRIG_PIN 10
#define ECHO_PIN 11
#define ENA 3
#define IN1 4
#define IN2 5
#define ENB 6
#define IN3 7
#define IN4 8
#define DHTPIN 12
#define DHTTYPE DHT11

// Create sensor and actuator objects
NewPing sonar(TRIG_PIN, ECHO_PIN);
Servo myServo;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(A0, INPUT);

  // Start serial communication
  Serial.begin(9600);

  // Initialize sensors and actuators
  dht.begin();
  myServo.attach(9);
}

void loop() {
  // Wait for a second between loops
  delay(1000);

  // Read humidity and temperature from DHT11 sensor
  humidity = dht.readHumidity();
  tempCelsius = dht.readTemperature();
  tempFahrenheit = dht.readTemperature(true);

  // Check if readings are valid
  if (isnan(humidity) || isnan(tempCelsius) || isnan(tempFahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    // Print humidity and temperature to serial monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Temperature: ");
    Serial.print(tempCelsius);
    Serial.println("C");
    Serial.print("Temperature: ");
    Serial.print(tempFahrenheit);
    Serial.println("F");
  }

  // Get distance reading from sonar
  distance = sonar.ping_cm();

  // Print distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control pump based on distance
  if (distance >= 12) {
    analogWrite(ENA, 255);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    analogWrite(ENA, 0);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // Read light sensor value
  lightSensorValue = analogRead(A0);
  Serial.print("Light Sensor Value: ");
  Serial.println(lightSensorValue);

  // Control servo based on light sensor value
  if (lightSensorValue >= 40) {
    myServo.write(0);
  } else if (lightSensorValue >= 30) {
    myServo.write(30);
  } else if (lightSensorValue >= 20) {
    myServo.write(60);
  } else if (lightSensorValue >= 10) {
    myServo.write(70);
  } else {
    myServo.write(90);
  }
}
