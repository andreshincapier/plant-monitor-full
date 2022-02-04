#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "time.h"

const char* ssid = "hincapieperafan";
const char* password =  "1040749227";

unsigned long epochTime; 

const char* ntpServer = "pool.ntp.org";
const char* serverName = "";

StaticJsonDocument<500> doc;

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  configTime(0, 0, ntpServer);
}

void loop() {
  Serial.println("Posting...");
  epochTime = getTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  doc["sensors"]["timestamp"] = epochTime;
  POSTData();
}


// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return (0);
  }
  time(&now);
  return now;
}


void POSTData() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String json;
    serializeJson(doc, json);

    Serial.println(json);
    int httpResponseCode = http.POST(json);
    Serial.println(httpResponseCode);
  }
}
