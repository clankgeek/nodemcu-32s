#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>

#define MAX_DEVICES 4 // 4 blocks
#define CS_PIN 17

#define WIFI_SSID "Use WIFI SSID"
#define WIFI_PASSWORD "Password to connect WIFI"

#define NTP_SERVER "fr.pool.ntp.org"
#define NTP_GMTOFFSET_SEC 7200 // Paris GMT + 2 = 3600 x 2

MD_Parola ledMatrix = MD_Parola(MD_MAX72XX::FC16_HW, CS_PIN, MAX_DEVICES);
char sep = true;
struct tm timeinfo;

void setup() {
  ledMatrix.begin();         // initialize the object
  ledMatrix.setIntensity(0); // set the brightness of the LED matrix display (from 0 to 15)
  ledMatrix.displayClear();  // clear LED matrix display
  ledMatrix.setTextAlignment(PA_CENTER); // center text
  delay(100);

  // Connect to wifi  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    ledMatrix.print("Wifi...");
    delay(100);
  }

  // NTP server
  configTime(NTP_GMTOFFSET_SEC, 0, NTP_SERVER);
  while(!getLocalTime(&timeinfo)) {
    ledMatrix.print("NTP...");
    delay(100);
  }

  // Disable wifi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  setCpuFrequencyMhz(80);
}

void loop() {
  getLocalTime(&timeinfo);
  if(sep) {
    ledMatrix.print(&timeinfo, "%H:%M");
  } else {
    ledMatrix.print(&timeinfo, "%H %M");
  }
  sep = !sep;
  delay(1000);
}