#include <Arduino.h>
#include <EEPROM.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

/*------------------------------------------------------------------------------------*/
/* Constant Definitions                                                               */
/*------------------------------------------------------------------------------------*/
// Access point to configure Wi-Fi
const char* ACCESS_POINT_NAME = "ESP8266";
const char* ACCESS_POINT_PASS = "esp8266";

/*------------------------------------------------------------------------------------*/
/* GPIO Definitions                                                                   */
/*------------------------------------------------------------------------------------*/
const uint8_t GPIO_ANALOG_00 = 0;          // ESP8266 NodeMCU A0
const uint8_t GPIO_UNUSED_00 = 0;          // ESP8266 NodeMCU D3
const uint8_t GPIO_UNUSED_01 = 1;          // ESP8266 NodeMUC TX (UART)
const uint8_t GPIO_UNUSED_02 = 2;          // ESP8266 NodeMUC D4 (Boot mode. Do not user for INPUT)
const uint8_t GPIO_UNUSED_03 = 3;          // ESP8266 NodeMCU RX (UART)
const uint8_t GPIO_DISPLAY_SDA = 4;        // ESP8266 NodeMCU D2 (SDA) 
const uint8_t GPIO_DISPLAY_SCL = 5;        // ESP8266 NodeMCU D1 (SCL)
const uint8_t GPIO_UNUSED_06 = 6;          // ESP8266 NodeMCU     + F M
const uint8_t GPIO_UNUSED_07 = 7;          // ESP8266 NodeMCU     + L E
const uint8_t GPIO_UNUSED_08 = 8;          // ESP8266 NodeMCU     + A M
const uint8_t GPIO_UNUSED_09 = 9;          // ESP8266 NodeMCU SD2 + S O
const uint8_t GPIO_UNUSED_10 = 10;         // ESP8266 NodeMCU SD3 + H R
const uint8_t GPIO_UNUSED_11 = 11;         // ESP8266 NodeMCU     +   Y
const uint8_t GPIO_UNUSED_12 = 12;         // ESP8266 NodeMCU D6
const uint8_t GPIO_UNUSED_13 = 13;         // ESP8266 NodeMUC D7
const uint8_t GPIO_UNUSED_14 = 14;         // ESP8266 NodeMCU D5
const uint8_t GPIO_UNUSED_15 = 15;         // ESP8266 NodeMCU D8 (Boot from SD Card)
const uint8_t GPIO_UNUSED_16 = 16;         // ESP8266 NodeMCU D0 (AKA LED_BUILTIN)

/*------------------------------------------------------------------------------------*/
/* Global Variables                                                                   */
/*------------------------------------------------------------------------------------*/
// WiFi Manager
WiFiManager wifiManager;

/*------------------------------------------------------------------------------------*/
/* WiFi Manager Global Functions                                                      */
/*------------------------------------------------------------------------------------*/
// WiFiManager Configuration CallBack
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("[WIFI]: Entered config mode");
  Serial.print("[WIFI]:"); Serial.println(WiFi.softAPIP());
  Serial.printf("[WIFI]: %s", (myWiFiManager->getConfigPortalSSID()).c_str());
}


void setup() {
  Serial.begin(115200);

  // Instantiate and setup WiFiManager
  // wifiManager.resetSettings(); Uncomment to reset wifi settings
  wifiManager.setAPCallback(configModeCallback);
  if (!wifiManager.autoConnect(ACCESS_POINT_NAME, ACCESS_POINT_PASS)) {
    Serial.println("Failed to connect and hit timeout");
    ESP.reset();
    delay(1000);  
  }

  // Config time
  setenv("TZ", "EST5EDT,M3.2.0/02:00:00,M11.1.0/02:00:00", 1);
  configTime(0, 0, "pool.ntp.org");

  // Initialize OTA (Over the air) update
  ArduinoOTA.setHostname(ACCESS_POINT_NAME);
  ArduinoOTA.setPassword(ACCESS_POINT_PASS);

  ArduinoOTA.onStart([]() {
    Serial.println("[OTA]: Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("[OTA]: End");
  });
  ArduinoOTA.onProgress([](uint32_t progress, uint32_t total) {
    Serial.printf("[OTA]: Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("[OTA]: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("[OTA]: Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("[OTA]: Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("[OTA]: Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("[OTA]: Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("[OTA]: End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("[OTA]: Ready");
}

void loop() {
  // OTA
  ArduinoOTA.handle();
  
}