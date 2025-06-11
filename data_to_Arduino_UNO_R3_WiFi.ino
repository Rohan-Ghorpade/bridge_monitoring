#include <HX711.h>
#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SoftwareSerial.h>
#define DHTPIN 5
#define DHTTYPE DHT22
// Pin Definitions
#define FLEX_SENSOR A3
#define WATER_LEVEL_SENSOR A1
#define VIBRATION_SENSOR A0
#define LOADCELL_DOUT_PIN 11
#define LOADCELL_SCK_PIN 12
#define IR_SENSOR_PIN4 // IR sensor pin
#define LDR_PIN A2 // LDR pin
#define BUZZER 6
#define RED_LED 8
#define GREEN_LED 7
#define SERVO_PIN 13
#define WHITE_LED 10
// Threshold Constants
#define FLEX_THRESHOLD 800
#define WATER_LEVEL_THRESHOLD 500
#define TEMPERATURE_THRESHOLD 40.0
#define VIBRATION_THRESHOLD 400
#define WEIGHT_THRESHOLD 100
#define ACCELERATION_THRESHOLD 5.0
#define STRUCTURAL_Z_VARIATION 3.0
#define LDR_THRESHOLD200 // Adjust based on ambientlight conditions
// Calibration Factor for Load Cell
#define CALIBRATION_FACTOR 482.03
// Objects
DHT dht(DHTPIN, DHTTYPE);
HX711 scale;
Servo servo;
Adafruit_MPU6050 mpu;
// SoftwareSerial for communication with ESP8266
SoftwareSerial espSerial(2, 3); // RX, TX
// System State
bool previousUnsafeState = false;
String safetyReason = "";
float maxWeight = 0.0;
float resetThreshold = 5.0;
void setup() {
Serial.begin(115200);
espSerial.begin(9600);
Serial.println("Bridge Health Monitoring System Initialized");
// Initialize Sensors
dht.begin();
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
scale.set_scale(CALIBRATION_FACTOR);
scale.tare();
if (!mpu.begin()) {
Serial.println("MPU6050 initialization failed! Check your connections.");
while (1);
}
// Initialize Actuators
servo.attach(SERVO_PIN);
servo.write(0); // Close the barrier initially
// Pin Modes
pinMode(FLEX_SENSOR, INPUT);
pinMode(WATER_LEVEL_SENSOR, INPUT);
pinMode(IR_SENSOR_PIN, INPUT); // IR sensor
pinMode(LDR_PIN, INPUT); // LDR
pinMode(BUZZER, OUTPUT);
pinMode(RED_LED, OUTPUT);
pinMode(GREEN_LED, OUTPUT);
Serial.println("System Ready. Monitoring...");
}
void loop() {
// Read Sensors
int flexValue = analogRead(FLEX_SENSOR);
int waterLevel = analogRead(WATER_LEVEL_SENSOR);
int vibrationValue = analogRead(VIBRATION_SENSOR);
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();
float currentWeight = scale.get_units(10);
// IR Sensor
bool obstacleDetected = digitalRead(IR_SENSOR_PIN);
// LDR Sensor
int ldrValue = analogRead(LDR_PIN);
// MPU6050 Data
sensors_event_t a, g, temp;
mpu.getEvent(&a,&g, &temp);
float accelX = a.acceleration.x;
float accelY = a.acceleration.y;
float accelZ = a.acceleration.z;
// Print Sensor Values
printSensorReadings(flexValue, waterLevel, vibrationValue, temperature, humidity, currentWeight,
accelX, accelY, accelZ, obstacleDetected, ldrValue);
// Evaluate Safety Conditions
bool unsafe = evaluateSafety(flexValue, waterLevel, vibrationValue,temperature, currentWeight,
accelX, accelY, accelZ, obstacleDetected);
// Control Actuators
if(unsafe != previousUnsafeState){
updateActuators(unsafe);
previousUnsafeState = unsafe;
}
// Control Lights based on LDR
controlLights(ldrValue);
delay(5000);
}
void printSensorReadings(int flex, int waterLevel, int vibration, float temp, float humidity, float
weight, float accelX, float accelY, float accelZ, bool obstacle, int ldr) {
Serial.println("===== Sensor Readings =====");
Serial.print("Flex:");Serial.println(flex);
Serial.print("Water Level:");Serial.println(waterLevel);
Serial.print("Vibration: "); Serial.println(vibration);
Serial.print("Temperature: "); Serial.print(temp); Serial.println(" °C");
Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
Serial.print("Weight:");Serial.print(weight);Serial.println(" kg");
Serial.print("Accel (X, Y, Z): ");
Serial.print(accelX); Serial.print(", ");
Serial.print(accelY); Serial.print(", ");
Serial.println(accelZ);
Serial.print("Obstacle Detected: ");Serial.println(obstacle ? "Yes": "No");
Serial.print("LDR Value:");Serial.println(ldr);
Serial.println("===========================");
}
bool evaluateSafety(int flex, int waterLevel, int vibration, float temp, float weight, float accelX, float
accelY, float accelZ, bool obstacle) {
bool unsafe = false;
safetyReason = "";
if (flex > FLEX_THRESHOLD) {
unsafe = true;
safetyReason += "Excessive bending;";
}
if(vibration > VIBRATION_THRESHOLD) {
unsafe = true;
safetyReason += "Excessive vibrations; ";
}
if (waterLevel > WATER_LEVEL_THRESHOLD) {
unsafe = true;
safetyReason += "High water level;";
}
if(temp > TEMPERATURE_THRESHOLD) {
unsafe = true;
safetyReason += "High temperature; ";
}
if(weight > WEIGHT_THRESHOLD) {
unsafe = true;
safetyReason += "Excessive load; ";
}
if (obstacle) {
unsafe = true;
safetyReason += "Obstacle detected;";
}
if (abs(accelX) > ACCELERATION_THRESHOLD || abs(accelY) >
ACCELERATION_THRESHOLD || abs(accelZ - 9.8) > STRUCTURAL_Z_VARIATION) {
unsafe = true;
safetyReason += "Structural instability;";
}
Serial.println(unsafe ? "Status: Unsafe Condition Detected!" : "Status: Safe to Cross");
Serial.println("Reason:" + safetyReason);
return unsafe;
}
void updateActuators(bool unsafe) {
if (unsafe) {
digitalWrite(RED_LED, HIGH);
digitalWrite(GREEN_LED, LOW);
digitalWrite(BUZZER, HIGH);
servo.write(0); // Open the barrier
} else {
digitalWrite(RED_LED, LOW);
digitalWrite(GREEN_LED, HIGH);
digitalWrite(BUZZER, LOW);
servo.write(90); // Close the barrier
}
}
void controlLights(int ldrValue) {
if (ldrValue < LDR_THRESHOLD) {
digitalWrite(WHITE_LED, HIGH); // Turn on bridge lights
} else {
digitalWrite(WHITE_LED, LOW); // Turn off bridge lights
}
}