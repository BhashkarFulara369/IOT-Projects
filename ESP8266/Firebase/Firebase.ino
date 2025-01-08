/*  */
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "ThingSpeak.h"
#include <SoftwareSerial.h>
#include <String.h>
#include <stdlib.h>


// WiFi credentials
const char* WIFI_SSID = "Wi-Fi_SSID";
const char* WIFI_PASSWORD = "PASSWORD";

// Firebase configuration
const char* API_KEY = "YOUR_API_KEY";
const char* DATABASE_URL = "YOUR_RTDB_URL";
const char* USER_EMAIL = "USER_EMAIL";
const char* USER_PASSWORD = "PASSWORD";


// DHT sensor configuration
#define DHTPIN 0 // D3 pin ESP8266
DHT dht(DHTPIN, DHT11);

// Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// LED pin configuration
const int ledPin = 4; // D2 Pin in ESP8266
const int ledPin2 = 5; // D1 Pin in ESP8266

WiFiClient client;
// pHValue is global because i am using 
float global_pHValue;


void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Initialize Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;


  // Initialize DHT sensor
  dht.begin();

}






void handleSerialCommunication() {
  if (Serial.available() > 0) {
    String rs = Serial.readStringUntil('\n');
    float pHValue = rs.toFloat();
    Serial.printf("%f\n", pHValue);
    global_pHValue = pHValue;
  }
}

void sendDataToFirebase() {
  int ledState;
  int ledState2;

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.printf("Temperature: %s\n", Firebase.RTDB.setFloat(&fbdo, F("/hydro/temp"), t) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Humidity: %s\n", Firebase.RTDB.setFloat(&fbdo, F("/hydro/humidity"), h) ? "ok" : fbdo.errorReason().c_str());

  if (Firebase.RTDB.getInt(&fbdo, "/led/state", &ledState)) {
    digitalWrite(ledPin, ledState);
    Serial.println("LED State: " + String(ledState));
  } else {
    Serial.println(fbdo.errorReason().c_str());
  }

   if (Firebase.RTDB.getInt(&fbdo, "/second/state", &ledState2)) {
    digitalWrite(ledPin2, ledState2);
    Serial.println("LED State: " + String(ledState2));
  } else {
    Serial.println(fbdo.errorReason().c_str());
  }

  static unsigned long last_pH_Send = 0;
  if (millis() - last_pH_Send >= 10000) {
    Serial.printf("p.H: %s\n,", Firebase.RTDB.setFloat(&fbdo, F("/hydro/pH"), global_pHValue) ? "ok" : fbdo.errorReason().c_str());
    last_pH_Send = millis();
  }
}

// run again and again it 
void loop() {
  handleSerialCommunication(); // Different Function for handling the Serial Communication 
  if (Firebase.ready()) {
    sendDataToFirebase();
  }
  delay(1000);
}


