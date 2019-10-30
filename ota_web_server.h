#ifndef OTA_WEB_SERVER_H
#define OTA_WEB_SERVER_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#define USE_SERIAL Serial

class ota_web_server{
  private:
        char* userTopic;
        bool statusStart = false;
  public:
        ota_web_server(char* userTopic);
        ~ota_web_server();
        const char* mqtt_server = "soldier.cloudmqtt.com";
        const int mqtt_port = 10222;
        const char* MQTT_USER = "qfuuiuqn";
        const char* MQTT_PASSWORD = "5VBORdUuUPFM";
        char* mqtt_status_publish = userTopic;
        
        String msg;
        bool setupStart(bool statusStart = false);
        void loop();
        

    
  
};
#endif
