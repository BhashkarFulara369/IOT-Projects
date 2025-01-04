/* This program sends '0' and '1' to the Firebase Real-Time Database to turn ON/OFF any electrical device. */

// Include all the above necessary libraries
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

/* Define the WiFi credentials */
#define WIFI_SSID "Wifi-SSID"
#define WIFI_PASSWORD "PASSWORD"

/*  Define the API Key */
#define API_KEY "In the firebase service account section you get it"

/* Define the RTDB URL */
#define DATABASE_URL "Your Real time Database URL" 

/* Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "In authentication section add the user"
#define USER_PASSWORD "Enter the password"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ledPin = 4;

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // IMPORTANT
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 /, 1024 / Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
}

void loop()
{
  // Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

  int ledState;
   if(Firebase.RTDB.getInt(&fbdo, "/led/state", &ledState)){
    digitalWrite(ledPin, ledState);
    Serial.println(ledState);
    Serial.println("Data Send to Firebase");
   }else{
    Serial.println(fbdo.errorReason().c_str());
   }
  }
}
