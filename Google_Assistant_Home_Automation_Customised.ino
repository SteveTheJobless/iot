#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4
#define A1 D5
#define A2 D6
#define A3 D7
#define A4 D8

#define WLAN_SSID       ""             // Your SSID
#define WLAN_PASS       ""        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "" //Adafruit Server
#define AIO_SERVERPORT                    
#define AIO_USERNAME    ""            // Username
#define AIO_KEY         ""   // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay1"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay4");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  


  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light4);
}

void loop() {
 
  MQTT_connect();
//Input Check
int bs1,bs2,bs3,bs4=0;  


  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    bs1=digitalRead(A1);
    bs2=digitalRead(A2);
    bs3=digitalRead(A3);
    bs4=digitalRead(A4);
    Serial.println("Button State 1= "+bs1);
    Serial.println("Button State 2= "+bs1);
    Serial.println("Button State 3= "+bs1);
    Serial.println("Button State 4= "+bs1);
    if (subscription == &Light1) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light1.lastread);
      int Light1_State = atoi((char *)Light1.lastread);
      if(bs1==0){
      digitalWrite(Relay1, Light1_State);
      }
      if(bs1==1){
        Light1_State=1-Light1_State;
        Serial.println("Inverse= "+Light1_State);
        digitalWrite(Relay1, Light1_State);
      }
      
    }
    if (subscription == &Light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light2.lastread);
      int Light2_State = atoi((char *)Light2.lastread);
      if(bs2==0){
      digitalWrite(Relay2, Light2_State);
      }
      if(bs2==1){
        Light2_State=1-Light2_State;
        Serial.println("Inverse= "+Light2_State);
        digitalWrite(Relay2, Light2_State);
      }
    }
    if (subscription == &Light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      int Light3_State = atoi((char *)Light3.lastread);
      if(bs3==0){
      digitalWrite(Relay3, Light3_State);
      }
      if(bs3==1){
        Light3_State=1-Light3_State;
        Serial.println("Inverse= "+Light3_State);
        digitalWrite(Relay3, Light3_State);
      }
    }
    if (subscription == &Light4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light4.lastread);
      int Light4_State = atoi((char *)Light4.lastread);
      if(bs4==0){
      digitalWrite(Relay4, Light4_State);
      }
      if(bs4==1){
        Light4_State=1-Light4_State;
        Serial.println("Inverse= "+Light4_State);
        digitalWrite(Relay4, Light4_State);
      }
    }
  }

  
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
