/*
    *** WiFi related operations ***

          connectWiFi:
            Connect to WLAN

          setup_WiFi:
            Initialize WiFi systems
*/

#pragma once
// Wi-Fi
#include <WiFi.h>
// Server
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class PX_WEBSERVER;

class PX_WiFi
{
private:
  // AP preliminary
  char AP_ssid[50];
  // STA preliminary
  String STA_ssid;
  String STA_pass;
  // V V V V V Save to disk
  char STA_IP_URL[50];
  IPAddress STA_IP;
  bool connected2WiFi = false;
  // Timers
  unsigned long NETWORK_t0 = millis();
  PX_WEBSERVER *webserverDriver;

  void connectWiFi(AsyncWebServerRequest *request = nullptr);

public:
  PX_WiFi();

  void setDrivers(PX_WEBSERVER *webserverDriver);
  void begin();
  bool getConnected2Wifi();
  char *getSTA_IP_URL();
  void WiFiStationConnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info);
  void WiFiStationDisconnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info);
};