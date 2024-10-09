#include <PX_NETWORK/PX_WIFI.h>
#include <UF0_DBG.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <PX_OS/PX_BLACKMAGIC.h>
#include <PX_HARDWARE/PX_POWER.h>
#include <PX_HARDWARE/PX_GYRO.h>
#include <PX_NETWORK/PX_WEBSERVER.h>

PX_WiFi::PX_WiFi() {}

void PX_WiFi::connectWiFi(AsyncWebServerRequest *request)
{
    // STA_ssid = request->arg("ssid");
    // STA_pass = request->arg("password");
    STA_ssid = "Oxo Fanarii";
    STA_pass = "internet";
    // STA_ssid = "Mafsolium";
    // STA_pass = "987654321";
    DBG(F("Credentials entered: UN ="), STA_ssid, "PW =", STA_pass);
    // Opening Station mode
    DBG(F("Initializing Wi-Fi station mode"));
    WiFi.begin(STA_ssid, STA_pass);
    DBG(F("Establishing Wi-Fi connection to "), STA_ssid);
    while (WiFi.status() != connected2WiFi)
    {
        if ((millis() - NETWORK_t0) > 300)
        {
            Serial.print(".");
            NETWORK_t0 = millis();
        }
    }
    snprintf(AP_ssid, sizeof(AP_ssid), "%d.%d.%d.%d", STA_IP[0], STA_IP[1], STA_IP[2], STA_IP[3]);
    snprintf(STA_IP_URL, sizeof(STA_IP_URL), "http://%s/jamm~!", AP_ssid);
}

void PX_WiFi::setDrivers(PX_WEBSERVER *serverDriver)
{
    this->webserverDriver = serverDriver;
}

void PX_WiFi::begin()
{
    // Init & connect WiFi
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.onEvent([this](WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
                 { this->WiFiStationConnected(wifi_event, wifi_info); }, ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent([this](WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
                 { this->WiFiStationDisconnected(wifi_event, wifi_info); }, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    // Start AP mode & setup server to enter credentials
    WiFi.softAP("PX Setup");
    // Start server
    DBG(F("Start the setup server"));
    webserverDriver->controlServer.begin();
    // Connect to WiFi
    connectWiFi();
    delay(1000);
}

void PX_WiFi::WiFiStationConnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
    DBG(F("\nConnected to the dashing \'*oO"), STA_ssid, "Oo*\'");
    // might need some delay for status() to update correctly
    connected2WiFi = WiFi.status();
    WiFi.softAPdisconnect(true);
}

void PX_WiFi::WiFiStationDisconnected(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info)
{
    DBG(F("Disconnected from the smashing \'*oO"), STA_ssid, "Oo*\'");
    connected2WiFi = WiFi.status();
    DBG(F("Attempting to reconnect.."));
    WiFi.reconnect();
}

bool PX_WiFi::getConnected2Wifi()
{
    return connected2WiFi;
}

char *PX_WiFi::getSTA_IP_URL()
{
    return STA_IP_URL;
}