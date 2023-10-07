
#include <WiFi.h>
#include <HTTPClient.h>
#include <arduino-timer.h>

// Credential
#include "authkey.h"

// Insert network credentials
//#define WIFI_SSID ""
//#define WIFI_PASSWORD ""

// LED rates
static const int rate_1s = 1000;  // ms
static const int LED_PIN = 2;
static const int BTN_PIN = 0;

auto timer = timer_create_default(); // create a timer with default settings

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
bool toggleLED(void *) {
  if (getLed() == "1") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED on");
  }
  else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED off");
  }
  return true;
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

  timer.every(1000, toggleLED);
}

void loop() {
  timer.tick();
}