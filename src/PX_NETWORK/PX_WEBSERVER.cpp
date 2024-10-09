#include <PX_NETWORK/PX_WEBSERVER.h>
#include <UF0_DBG.h>
#include <PX_HARDWARE/PX_POWER.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <PX_OS/PX_BLACKMAGIC.h>
#include <PX_HARDWARE/PX_GYRO.h>
#include <PX_NETWORK/PX_WIFI.h>

// Web pages (HTML)
#include <PX_NETWORK/PX_WEBPAGES/transmitGyroPage.h>

AsyncWebServer PX_WEBSERVER::controlServer = AsyncWebServer(80);

PX_WEBSERVER::PX_WEBSERVER()
{
    this->callback = [this](const String &s)
    { return this->HTMLprocessor(s); };
}

void PX_WEBSERVER::setDrivers(PX_POWER *powerDriver,
                              PX_MIDI *midiDriver,
                              PX_BLACKMAGIC *blackmagic,
                              PX_GYRO *gyroDriver,
                              PX_WiFi *wifiDriver)
{
    this->powerDriver = powerDriver;
    this->midiDriver = midiDriver;
    this->blackmagic = blackmagic;
    this->gyroDriver = gyroDriver;
    this->wifiDriver = wifiDriver;
}

// Setup WebServer
void PX_WEBSERVER::begin()
{
    controlServerRequests();
}

void PX_WEBSERVER::loop()
{
    sprintf(htmlBuffer, transmitGyroHTML, 15.0f);
}

// Propagate data to HTML
String PX_WEBSERVER::HTMLprocessor(const String &var)
{
    if (var == "pitchTransmit")
    {
        return (midiDriver->getPitchTransmission() ? "On" : "Off");
    }
    else if (var == "rollTransmit")
    {
        return (midiDriver->getRollTransmission() ? "On" : "Off");
    }
    else if (var == "yawTransmit")
    {
        return (midiDriver->getYawTransmission() ? "On" : "Off");
    }
    else if (var == "isConnected")
    {
        return (wifiDriver->getConnected2Wifi() ? "Connected to WiFi!" : "Not connected to WiFi");
    }
    return "fail";
}

void PX_WEBSERVER::setupMIDIHandlers()
{
    controlServer.on("/startX", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					DBG("pitch transmit on:", midiDriver->getPitchTransmission());
                        midiDriver->startPitchTransmission();
                        request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/startY", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					DBG("roll transmit on:", midiDriver->getRollTransmission());
                        midiDriver->startRollTransmission();
                        request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/startZ", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					DBG("yaw transmit on:", midiDriver->getYawTransmission());
                        midiDriver->startYawTransmission();
                        request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/stopX", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					DBG("pitch transmit off:", midiDriver->getPitchTransmission());
                        midiDriver->stopPitchTransmission();
                        request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/stopY", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					DBG("roll transmit off:", midiDriver->getRollTransmission());
                        midiDriver->stopRollTransmission();
                        request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/stopZ", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					DBG("yaw transmit off:", midiDriver->getYawTransmission());
                        midiDriver->stopYawTransmission();
                        request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/soloX", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					midiDriver->soloPitchTransmission();
    					request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/soloY", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					midiDriver->soloRollTransmission();
    					request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/soloZ", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					midiDriver->soloYawTransmission();
    					request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/enableAll", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					midiDriver->startTransmissionAll();
    					request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/disableAll", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					midiDriver->stopTransmissionAll();
    					request->send_P(200, "text/html", transmitGyroHTML, callback); });
}

void PX_WEBSERVER::setupSaveLoadHandlers()
{
    controlServer.on("/save", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					 blackmagic->savePreset();
                         request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/load", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
    					 blackmagic->loadPreset();
    					 request->send_P(200, "text/html", transmitGyroHTML, callback); });
}

void PX_WEBSERVER::setupUpdateHandlers()
{
    controlServer.on("/updateX", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { request->send(200, "text/plain", (this->midiDriver->getPitchTransmission() ? "On" : "Off")); });

    controlServer.on("/updateY", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { request->send(200, "text/plain", (this->midiDriver->getRollTransmission() ? "On" : "Off")); });

    controlServer.on("/updateZ", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { request->send(200, "text/plain", (this->midiDriver->getYawTransmission() ? "On" : "Off")); });
}

void PX_WEBSERVER::controlServerRequests()
{
    DBG("WFFW333", powerDriver->get_battery_percent());
    controlServer.onNotFound([](AsyncWebServerRequest *request)
                             { request->send(404, "text/plain", "Not found"); });

    controlServer.on("/", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
                         /////////////////////////////////////////////////////////
                         String batteryStr = String(powerDriver->get_battery_percent());
                         request->send(200, "text/plain", batteryStr);
                         /////////////////////////////////////////////////////////

                         // request->send_P(200, "text/html", credentialsHTML, HTMLprocessor);
                     });

    controlServer.on("/connect", HTTP_GET, [](AsyncWebServerRequest *request)
                     {
                         // connectWiFi(request);
                     });

    controlServer.on("/redirect", HTTP_GET, [this](AsyncWebServerRequest *request)
                     {
                         request->redirect(wifiDriver->getSTA_IP_URL());
                         // Go incognitto (close PX Setup AP)
                         // WiFi.softAPdisconnect(connected2WiFi);
                         // DBG("Disconnected Setup AP");
                     });

    controlServer.on("/jam", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { request->send_P(200, "text/html", transmitGyroHTML, callback); });

    controlServer.on("/updateWiFi", HTTP_GET, [this](AsyncWebServerRequest *request)
                     { request->send(200, "text/plain", (wifiDriver->getConnected2Wifi() ? "Connected to WiFi!" : "Not connected to WiFi")); });

    setupMIDIHandlers();
    setupSaveLoadHandlers();
    setupUpdateHandlers();
}