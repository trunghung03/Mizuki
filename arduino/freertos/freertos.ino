// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#include <WiFi.h>
#include <HTTPClient.h>


// Credential
#include "authkey.h"

// Insert network credentials
//#define WIFI_SSID ""
//#define WIFI_PASSWORD ""

// LED rates
static const int rate_1s = 1000;  // ms
static const int LED_PIN = 2;


//#define URLLED = "";

// GET request to Firebase that returns led status as string, 0 or 1
String getLed() {
  HTTPClient http;
  http.begin(URLLED);
  http.GET();
  String result = http.getString();
  http.end();
  return result;
}

// FreeRTOS task
// Toggle LED based on result of request
void toggleLED(void *parameter) {
  if (getLed() == "1") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED on");
  }
  else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED off");
  }
  vTaskDelay(rate_1s / portTICK_PERIOD_MS);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // Wifi init
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.print("\nOK! IP=");
  Serial.println(WiFi.localIP());


    // Task to run forever
  xTaskCreatePinnedToCore(
              toggleLED,  // Function to be called
              "Toggle led",
              8192,         // Stack size (bytes in ESP32, words in FreeRTOS), 4k or below got a stack overflow
              NULL,
              1,
              NULL,
              app_cpu);
}

void loop() {
}