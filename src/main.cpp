#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifimqtt.h"
// // put function declarations here:
// int myFunction(int, int);

byte led = 4;
unsigned long previousMillis = millis();

void sendMQTTvalues(){
  StaticJsonDocument<256> doc;

  doc["device"] = "ESP32 is the best MCU";
  doc["temperature"] = random(20, 35);
  doc["humedity"] = random(40, 90);
  doc["lux"] = random(100, 1000);

  char buff[256];
  serializeJson(doc, buff);
  client.publish("ESPValues", buff);
}

void setup() {
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(115200);
  pinMode(led, OUTPUT); 
  connectAP();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  if(!client.loop()){
    client.connect("ESP32.-");
  }
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 10000) {
    previousMillis = currentMillis;
    //digitalWrite(led, !digitalRead(led)); 
    // client.publish("toNodeRED","hello from ESP32");
    sendMQTTvalues();
  }
}

// put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }