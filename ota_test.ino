#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";
const char* update_url = "http://example.com/firmware.bin";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Serial.println("Connected!");
  HTTPClient http;
  http.begin(update_url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);
    if (canBegin) {
      size_t written = Update.writeStream(http.getStream());
      if (written == contentLength) {
        Serial.println("Written : " + String(written) + " successfully");
      } else {
        Serial.println("Write failed. Written only : " + String(written) + "/" + String(contentLength));
      }
      if (Update.end()) {
        if (Update.isFinished()) {
          Serial.println("Update successfully completed. Rebooting.");
          ESP.restart();
        } else {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      Serial.println("Not enough space to begin OTA");
    }
  } else {
    Serial.println("Failed to fetch update");
    http.end();
  }
}

void loop() {
  // Code here runs while the device is updating
}