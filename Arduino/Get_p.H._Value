/* This code is written for getting the ph reading fromn the ph sensor and send it to the esp8266 (wifi microcontroller) using the arduino mega rx and tx pins and displayin it in the serial monitor and lcd (16*2) display. (If you use display than you can use )*/
// Basic Code for diplaying 
#include <LiquidCrystal.h>;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2,ct = 9;
LiquidCrystal mylcd(rs, en, d4, d5, d6, d7);


// All Global Variables and their values 

float calibration = 22.49; //change this value to calibrate
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

// Setup function runs only one time

void setup() {
Serial.begin(115200);
analogWrite(ct,80);
mylcd.begin(16, 2);
mylcd.setCursor(0,0);
mylcd.print("Measureof-T-P-H");
mylcd.setCursor(3,1);    
mylcd.print("Value...");
delay(9000);
mylcd.clear();

}



// runs again and again
void loop() {
  //mylcd.clear();
for(int i=0;i<10;i++) 
{ 
buf[i]=analogRead(analogInPin);
delay(30);
}
for(int i=0;i<9;i++)
{
for(int j=i+1;j<10;j++)
{
if(buf[i]>buf[j])
{
temp=buf[i];
buf[i]=buf[j];
buf[j]=temp;
}
}
}
avgValue=0;
for(int i=2;i<8;i++)
avgValue+=buf[i];
float pHVol=(float)avgValue*5.0/1024/6;
float phValue = -5.70 * pHVol + calibration;
String sv = String(phValue);
Serial.println(sv);

 //mylcd.setCursor(0,0);  
// mylcd.print("pH_Value");
// mylcd.setCursor(3,1);  
//mylcd.print(phValue);

delay(15000);
}
