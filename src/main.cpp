#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "wifimqtt.h"
#include <DHT.h>
// // put function declarations here:
// int myFunction(int, int);

byte led = 4;
unsigned long previousMillis = 0;

// void sendMQTTvalues(){
//   StaticJsonDocument<256> doc;

//   doc["device"] = "ESP32 is the best MCU";
//   doc["temperature"] = random(20, 35);
//   doc["humedity"] = random(40, 90);
//   doc["lux"] = random(100, 1000);

//   char buff[256];
//   serializeJson(doc, buff);
//   client.publish("ESPValues", buff);
// }

#define DHTPIN 27          // Pin donde conectas el DHT
#define DHTTYPE DHT22      // DHT22 (o DHT11)

DHT dht(DHTPIN, DHTTYPE); // Clase DHT con el nombre de la variable dht para manejar el sensor DHT22

float temperature;
float humidity; 

void enviarValoresSensorMQTT(){
  StaticJsonDocument<256> doc;

  doc["device"] = "ESP32";
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;

  char buff[256];
  serializeJson(doc, buff);
  client.publish("estacion", buff);
}

void ImprimirSensorData(){
  if (!isnan(temperature) && !isnan(humidity)) { 
    
      Serial.print("Temp: ");
      Serial.print(temperature);
      Serial.println(" °C");
      Serial.print(" | Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      
  
  } else {
    Serial.println("Failed to read from DHT sensor!");
    }
}

void setup() {

  Serial.begin(115200); // Inicializa la comunicación serial
  dht.begin();
 
  // pinMode(led, OUTPUT); 
  connectAP(); // Conecta a la red WiFi
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect(); // Conecta al broker MQTT
}

void loop() {
  if(!client.connected()){ // Si no estamos conectados al broker MQTT, intentamos reconectar
    reconnect();
  }
  if(!client.loop()){
    client.connect("ESP32.-"); // Intentamos reconectar si la conexión se perdió
  }


  unsigned long currentMillis = millis();

  ////////////////////////////////////////////////////////////////////////////
  // Lectura de datos del DHT22 //
  if(currentMillis - previousMillis >=(1000*3)){
    previousMillis = currentMillis;
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    ImprimirSensorData();
    enviarValoresSensorMQTT();
  }
}


