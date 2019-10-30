/**
 * httpUpdate.ino
 *
 *  Created on: 27.11.2015
 *
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include "ota_web_server.h"
WiFiClient wifiClient;
PubSubClient client(wifiClient);
//ใส้ userTopic ที่ได้จาก web
char* userTopic = "------"; 
//ota
ota_web_server ota(userTopic);
// กำหนด wifi
const char* ssid = "-----"; // ชื่อ wifi
const char* password = "-----"; // รหัส wifi
///////////////////////////////////////////
const char* mqtt_server = ota.mqtt_server;
const int mqtt_port = ota.mqtt_port;
const char* MQTT_USER = ota.MQTT_USER;
const char* MQTT_PASSWORD  = ota.MQTT_PASSWORD;
char* mqtt_status_publish = ota.mqtt_status_publish;
//////////////////////////////////////////


void setup() {
  ////// setup ///////////////////////////////////////
    Serial.begin(115200);
    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    //// นับถอยหลัง 5 วินาที
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFi.begin(ssid, password);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    reconnect();
   //--------------------------------------------------//
}

void loop() {
    //////
    client.loop();
    ota.loop();
    // coding chill // 
    
    

    
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    //topics = topic;
    String msg2;
    Serial.print("data:");  
    Serial.write(payload, length);
    Serial.println();
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg2 += (char)payload[i];
  }
  
  Serial.println();
  Serial.print("msg2:");
  Serial.println(msg2);
  if(String(topic) == userTopic && msg2 == "check/running"){
//    
      delay(5000);
      client.publish(userTopic, "running");
//    ota.setupStart(true);
//    
//    Serial.println(ota.msg);
   
  }else if(String(topic) == userTopic && msg2 == "running"){
    Serial.println("running");
  }
  else{
    ota.msg = msg2;
    ota.setupStart(true);
//    
    Serial.println(ota.msg);
  }
}

void reconnect(){
  
   // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish(userTopic, "running");
      // ... and resubscribe
      client.subscribe(userTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  
}
