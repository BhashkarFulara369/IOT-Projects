
#include <DHT.h>  // Including library for dht
 //#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <ESP8266WiFi.h>
 #include<SoftwareSerial.h>
#include<stdlib.h>
#include<String.h>
String apiKey = "4890C7SBGCX1LBMH";     //  API KEY
 
const char *ssid =  "Bias TF B2";     
const char *pass =  "bias@123";
//const char* server = "api.thingspeak.com";
 unsigned long myChannelNumber = 2613670;
const char * myWriteAPIKey = "4890C7SBGCX1LBMH";

#define DHTPIN 0          //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);
 
WiFiClient client;



  
void setup() 
{
 
  
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

 WiFi.mode(WIFI_STA);
 ThingSpeak.begin(client);  // Initialize ThingSpeak
}
 
void loop() 
{
 
  // Serial communication for ard. to esp for ph data sending

 
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }
                      // set the fields with the values
                      // ESP8266 code
  // Receive pH value 
 String rs = Serial.readStringUntil('\n');
 

  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, rs);

    // set the status
 // ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.print("ph is - ");
                             Serial.print(rs);
                            // Serial.println(phValue); 
        //                    statusCode = ThingSpeak.getLastReadStatus();  //--> Check the status of the read operation to see if it was successful
  
                             Serial.println("%. Send to Thingspeak.");
                        
          
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
  delay(15000);
}





























