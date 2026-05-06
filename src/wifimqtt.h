#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "credentials.h"
// Configuración MQTT
String clientID = "ESP32-";
extern byte led;

const char* mqtt_server = "192.168.1.227";  // Broker MQTT público
const int mqtt_port = 1883;
const char* mqtt_user = "adminmarco";             // Usuario MQTT (vacío para broker público)
const char* mqtt_password = "sushanez#1234";         // Contraseña MQTT (vacío para broker público)
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect()
{
    while(!client.connected()){
        Serial.print("Intentando Conexion MQTT...");
        clientID+=String(random(0xffff), HEX);
        if (client.connect(clientID.c_str(), mqtt_user, mqtt_password)){
            Serial.println("Conectado");
            client.subscribe("estacion");
            // client.subscribe("ledState");
            // client.subscribe("JSONfromNodeRED");
        } else {
            Serial.print("Fallo, rc=");
            Serial.print(client.state());
            Serial.println(" Intentando de nuevo en 5 segundos");
            delay(5000);
        }
    }
};

void callback(char *topic, byte *message, unsigned int length){
    // Serial.print("Message arrived on topic: ");
    // Serial.print(topic);
    // Serial.print(". Message: ");
    String messageTemp;
    for (int i =0; i< length; i++){
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    if(String(topic) == "estacion"){
        // Stream& input;

        StaticJsonDocument<128> doc;

        DeserializationError error = deserializeJson(doc, messageTemp);

        if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
        }

        const char* device = doc["device"]; // "ESP32 is the best MCU"
        int temperature = doc["temperature"]; // 33
        int humidity = doc["humidity"]; // 90
        // int lux = doc["lux"]; // 800

        String output = String(device) + " "+String(temperature) + "°C "+String(humidity) + "% ";
        Serial.println(output);
    }

}

void connectAP(){
    Serial.print("Connecting to my WiFi");
    WiFi.begin(ssid, password);

    // WiFi.begin(ssid);
    byte cnt=0;
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.print(".");
        cnt++;
        if(cnt>30){
            ESP.restart();
        }
        Serial.println(WiFi.localIP());
    }
}