#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>// Replacewith your Wi-Fi credentials
const char* ssid = "name"; // Your Wi-Fi SSID
const char* password = "password"; // Your Wi-Fi password
SoftwareSerial arduinoSerial(D5, D6); // RX, TX for communication with Arduino
// Sensor data strings
String flexSensorData = "No Data";
String waterLevelData = "No Data";
String temperatureData = "No Data";
String humidityData = "No Data";
String accelData = "No Data";
String vibrationData = "No Data"; // New: Vibration sensor data
String loadData = "No Data"; // New: Load sensor data
String irSensorData = "No Data"; //New:IR sensor data
String ldrData = "No Data"; // New: LDR sensor data
// Thresholds for tilt detection
const float tiltThreshold = 5.0; // Any tilt above 5° is considered unsafe
const int LDR_THRESHOLD = 400; // Define LDR_THRESHOLD for low light conditions
ESP8266WebServer server(80); // Declare and initialize the web server object
void setup() {
Serial.begin(115200); //Start serial communication with the PC
arduinoSerial.begin(9600); // Start serial communication with Arduino
Serial.println("Waiting for serial data...");
// Connecting to Wi-Fi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
Serial.println("\nConnected to Wi-Fi");
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());
// Setup the server to handle requests on the root page
server.on("/", handleRoot); // Set the handler for the root path
server.begin(); // Start the web server
}
void loop() {
if (arduinoSerial.available()) {
String data = arduinoSerial.readStringUntil('\n');
Serial.println("Received data:" + data);
if(data.startsWith("FlexSensor:")){
flexSensorData = data.substring(13);
} else if(data.startsWith("Water Level:")){
waterLevelData = data.substring(13);
} else if(data.startsWith("Temperature:")) {
temperatureData = data.substring(13);
} else if(data.startsWith("Humidity:")){
humidityData = data.substring(9);
} else if(data.startsWith("Accelerometer (X, Y, Z):")){
accelData = data.substring(23);
} else if (data.startsWith("Vibration:")) {
vibrationData = data.substring(11);
} else if (data.startsWith("Weight (Load Sensor):")) {
loadData = data.substring(22);
} else if(data.startsWith("IR Sensor:")){ // Handle IR sensor data
irSensorData = data.substring(11);
} else if(data.startsWith("LDR Value:")){ //Handle LDR sensor data
ldrData = data.substring(10);
}
}
// Evaluate bridge safety
String reasons;
String status = evaluateBridgeSafety(reasons);
bool unsafe = status.startsWith("UNSAFE");
// Log status
Serial.println("Bridge Status: " + status);
Serial.println("Reasons:" + reasons);
server.handleClient(); // Handle any incoming web client requests
}
// Evaluate bridge safety based on sensor data
String evaluateBridgeSafety(String &reasons) {
float waterLevel = waterLevelData.toFloat();
floattemperature = temperatureData.toFloat();
float humidity = humidityData.toFloat();
float vibration = vibrationData.toFloat(); // Parse vibration data
float load = loadData.toFloat(); // Parse load data
float ldrValue = ldrData.toFloat(); // Parse LDR data
String status = "SAFE: Conditions are normal.";
reasons = ""; // Clear previous reasons
// Handle tilt condition from accelerometer data
String accelString = accelData;
float accelX = 0, accelY = 0, accelZ = 0;
if(accelString != "No Data"){
int firstComma = accelString.indexOf(',');
int secondComma = accelString.indexOf(',', firstComma + 1);
accelX = accelString.substring(0, firstComma).toFloat();
accelY = accelString.substring(firstComma + 1, secondComma).toFloat();
accelZ = accelString.substring(secondComma + 1).toFloat();
}
// Check if the tilt exceeds the threshold
floattilt = sqrt(sq(accelX) + sq(accelY)); // Using X and Y components for tilt (ignoring Z for
simplicity)
if (tilt > tiltThreshold) {
status = "UNSAFE: Tilt detected.";
reasons += "Structural instability detected. ";
}
// Check other sensor thresholds
if(vibration > 400){ // Add vibration threshold
status = "UNSAFE: Issues detected.";
reasons += "Excessive vibrations detected. ";
}
if(load > 100) { // Add load threshold
status = "UNSAFE: Issues detected.";
reasons += "Excessive load detected.";
}
if(waterLevel > 75){
status = "UNSAFE: Issues detected.";
reasons += "High water level detected.";
}
if (temperature > 50) {
status = "UNSAFE: Issues detected.";
reasons += "High temperature detected.";
}
if(humidity > 100){
  status = "UNSAFE: Issues detected.";
reasons += "High humidity levels detected.";
}
// IR sensor condition for obstacle detection
if (irSensorData == "ObstacleDetected"){ // IR sensor detects an obstacle
status = "UNSAFE: Issues detected.";
reasons += "Obstacle detected by IR sensor.";
}
if(status.startsWith("SAFE")){
reasons = "No issues detected.";
}
return status;
}
void handleRoot(){
String reasons;
String status = evaluateBridgeSafety(reasons);
String safeToCross = status.startsWith("SAFE") ? "Yes" :"No";
// Determine light status
String lightStatus = ldrData.toFloat() < LDR_THRESHOLD ? "OFF" : "ON";
String backgroundColor = safeToCross == "Yes" ? "#28a745" :"#dc3545";
String webpage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="refresh" content="1"> <!-- Refresh the page every 10 seconds -->
<title>Bridge Health Monitor</title>
<style>
body {
font-family: 'Arial', sans-serif;
margin: 0;
padding: 0;
background: #f0f4f8;
color: #333;
}
header {
text-align: center;
background: #0078d4;
color: white;
padding: 20px 0;
}
header h1 {
margin: 0;
}
.container {
max-width: 800px;
margin: 20px auto;
padding: 20px;
background: white;
border-radius: 10px;
box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}
.status {
text-align: center;
font-size: 1.5rem;
font-weight: bold;
padding: 15px;
border-radius: 5px;
color: white;
background-color:
)rawliteral" + backgroundColor + R"rawliteral(;
}
table {
width: 100%;
border-spacing: 0;
margin: 20px 0;
}
table th,table td {
padding: 10px;
text-align: left;
border-bottom: 1px solid #ddd;
}
table th {
background: #f8f9fa;
}
footer {
text-align: center;
padding: 10px 0;
background: #0078d4;
color: white;
position: fixed;
bottom: 0;
width: 100%;
}
.badge {
display: inline-block;
padding: 5px 10px;
border-radius: 3px;
color: white;
font-weight: bold;
font-size: 0.9rem;
}
.badge-safe {
background: #28a745;
}
.badge-unsafe {
  background: #dc3545;
}
</style>
</head>
<body>
<header>
<h1>Bridge Health Monitoring</h1>
</header>
<div class="container">
<div class="status">Bridge Status: <span class="badge badge-)rawliteral" + (safeToCross ==
"Yes" ? "safe" : "unsafe") + R"rawliteral(">)rawliteral" + status + R"rawliteral(</span></div>
<h2>Sensor Readings</h2>
<table>
<tr>
<th>Sensor</th>
<th>Value</th>
</tr>
<tr>
<td>FlexSensor</td>
<td>)rawliteral" + flexSensorData + R"rawliteral(</td>
</tr>
<tr>
<td>Water Level</td>
<td>)rawliteral" + waterLevelData + R"rawliteral(</td>
</tr>
<tr>
<td>Temperature</td>
<td>)rawliteral" + temperatureData + R"rawliteral(</td>
</tr>
<tr>
<td>Humidity</td>
<td>)rawliteral" + humidityData + R"rawliteral(</td>
</tr>
<tr>
<td>Load</td>
<td>)rawliteral" + loadData + R"rawliteral(</td>
</tr>
<tr>
<td>Accelerometer (X, Y, Z)</td>
<td>)rawliteral" + accelData + R"rawliteral(</td>
</tr>
<tr>
<td>IR Sensor</td>
<td>)rawliteral" + irSensorData + R"rawliteral(</td>
</tr>
<tr>
<td>LDR Value</td>
<td>)rawliteral" + ldrData + R"rawliteral(</td>
</tr>
<tr>
<td>Lights Status</td>
<td>)rawliteral" + lightStatus + R"rawliteral(</td>
</tr>
</table>
<h2>Analysis</h2>
<p><strong>Safe to Cross:</strong> )rawliteral" + safeToCross + R"rawliteral(</p>
<p><strong>Reasons:</strong> )rawliteral" + reasons + R"rawliteral(</p>
</div>
<footer>
&copy; 2024 Bridge Monitoring System | CMRIT
</footer>
</body>
</html>
)rawliteral";
server.send(200, "text/html", webpage);
}