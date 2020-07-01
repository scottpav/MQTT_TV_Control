/*
 *  ESP8266 NodeMCU MQTT Example
 *  https://circuits4you.com
 *  -Manoj R. Thakur
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#define LED 2
 
//Enter your wifi credentials
const char* ssid = "MailTruck";  
const char* password =  "Faith1212";

//Enter your mqtt server configurations
const char* mqttServer = "192.168.1.88";    //Enter Your mqttServer address
const int mqttPort = 1883;       //Port number
const char* mqttUser = ""; //User
const char* mqttPassword = ""; //Password
IRsend irsend(4);  // An IR LED is controlled by GPIO pin 4 (D2)

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  delay(1000);
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  irsend.begin();
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTTcallback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP8266", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
      digitalWrite(LED,HIGH);

    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());  //If you get state 5: mismatch in configuration
      delay(2000);
 
    }
  }
 
  client.publish("TVRemote", "Hello from ESP8266");
  client.subscribe("TVRemote");
}
 
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
  Serial.print(message);
  if(message == "#pwr") {irsend.sendNEC(0x40be629d, 32);}    //TV on  
  if(message == "#vup"){volumeUp();}                                               //Volume Up 
  if(message == "#vdown"){volumeDown();}                                           //Volume Down 
  if(message == "#mute"){irsend.sendNEC(0x40be32cd, 32);}  //Mute  
  if(message == "#src0"){sourceNext();}                                            //Source Next
  if(message == "#src2"){source2();}                                               //Source 2 
  if(message == "#src3"){source3();}                                               //Source 3 

  Serial.println();
  Serial.println("-----------------------");  
}

void volumeUp(){
   for (int i = 0; i < 5; i++) {
    irsend.sendNEC(0x40be30cf, 32); 
    delay(300);
   }
}

void volumeDown(){
   for (int i = 0; i < 5; i++) {
    irsend.sendNEC(0x40be9867, 32); 
    delay(300);
   }
}

void sourceNext(){
  irsend.sendNEC(0x40bed22d, 32); //Source
  delay(100);
  irsend.sendNEC(0x40be12ed, 32); //Up
  delay(100);
  irsend.sendNEC(0x40be52ad, 32); //Ok
}

void source2(){
  irsend.sendNEC(0x40bed22d, 32); //Source
  delay(100);
  irsend.sendNEC(0x40be12ed, 32); //Up
  delay(100);
  irsend.sendNEC(0x40be52ad, 32); //Ok
}

void source3(){
  irsend.sendNEC(0x40bed22d, 32); //Source
  delay(100);
  irsend.sendNEC(0x40beb24d, 32); //Down
  delay(100);
  irsend.sendNEC(0x40be52ad, 32); //Ok
}
void loop() {
  client.loop();
}
