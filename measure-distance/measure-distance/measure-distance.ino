#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "belik";
const char *password = "putra12345";
const char *serverUrl = "https://api-iot-testing.vercel.app/"; // Ganti dengan URL API yang sesuai

const int trigPin = 5;
const int echoPin = 18;

// define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  // Send data to server via API with distance as a query parameter
  sendDataToServer();

  delay(500);
}

void sendDataToServer() {
  HTTPClient http;

  // Prepare the URL with query parameters
  String urlWithParams = String(serverUrl) + "post-measure-distance?distance=" + String(distanceCm);

  // Make a POST request with the modified URL
  http.begin(urlWithParams);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST("{}");  // You can send an empty JSON or modify this based on your API requirements

  // Check for successful HTTP response
  if (httpCode > 0) {
    Serial.printf("HTTP response code: %d\n", httpCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.printf("HTTP request failed with error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
