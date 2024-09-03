#include <ESP8266SSDP.h>

#define BLYNK_TEMPLATE_NAME "NodeMCU"
#define BLYNK_AUTH_TOKEN "b9qFIrrnjhptGJ3bS4vaPBQ3NtwtJp6g"
#define BLYNK_TEMPLATE_ID "TMPL32r-xTRFd"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

//hardWare Specifics
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
char ssid[] =  "Prakhar"; 
char pass[] =  "prakhar@1210";     


bool fetch_blynk_state = true;  //true or false
#define wifiLed   LED_BUILTIN   //D0int 
int wifiFlag = 0;

// the virtual pins 
#define VPIN_SWITCH_1    V0 
#define VPIN_SWITCH_2    V1 
#define VPIN_SWITCH_3    V2
#define VPIN_SWITCH_4    V3
#define VPIN_SWITCH_5    V4

BlynkTimer timer;


BLYNK_WRITE(VPIN_SWITCH_1) 
{      
  Serial.write('f');
}
BLYNK_WRITE(VPIN_SWITCH_2) 
{ 
  Serial.write('b');     
}
BLYNK_WRITE(VPIN_SWITCH_3) 
{      
  Serial.write('l');
}
BLYNK_WRITE(VPIN_SWITCH_4) 
{      
  Serial.write('r');
}
BLYNK_WRITE(VPIN_SWITCH_5){
  if(VPIN_SWITCH_5 == 1){
    Serial.write('P');
    Serial.println('P');
  }
  else{
    Serial.write('p');
    Serial.println('p');
  }
  // Serial.println('P');
} 

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_SWITCH_1);
    Blynk.syncVirtual(VPIN_SWITCH_2);
    Blynk.syncVirtual(VPIN_SWITCH_3);
    Blynk.syncVirtual(VPIN_SWITCH_4);
    Blynk.syncVirtual(VPIN_SWITCH_5);
  }  
}

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    Serial.println("Blynk Not Connected");
  }
  if (isconnected == true) {
    if (!fetch_blynk_state){
    }
    
    wifiFlag = 0;
  }
}

void setup()
{     
       // Debug console
      Serial.begin(9600);
      
      pinMode(wifiLed, OUTPUT);

      Blynk.begin(auth, ssid, pass);

      timer.setInterval(2000L, checkBlynkStatus); 
      delay(1000);

}

void loop()
{
  Blynk.run();
  timer.run();

}