/*
    *** WebServer related operations ***

          PX_WEBSERVER class:

          PX_WiFi class:

          HTMLprocessor:
            Parse & process HTML placeholders
*/

#pragma once
// Server
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class PX_POWER;
class PX_MIDI;
class PX_BLACKMAGIC;
class PX_GYRO;
class PX_WiFi;

class PX_WEBSERVER
{
private:
    PX_POWER *powerDriver;
    PX_MIDI *midiDriver;
    PX_BLACKMAGIC *blackmagic;
    PX_GYRO *gyroDriver;
    PX_WiFi *wifiDriver;

    char htmlBuffer[2048];
    AwsTemplateProcessor callback;
    String HTMLprocessor(const String &var);
    void setupMIDIHandlers();
    void setupSaveLoadHandlers();
    void setupUpdateHandlers();
    void controlServerRequests();

public:
    PX_WEBSERVER();
    void setDrivers(PX_POWER *powerDriver,
                    PX_MIDI *midiDriver,
                    PX_BLACKMAGIC *blackmagic,
                    PX_GYRO *gyroDriver,
                    PX_WiFi *wifiDriver);

    // Setup WebServer
    static AsyncWebServer controlServer;
    void begin();
    void loop();
};