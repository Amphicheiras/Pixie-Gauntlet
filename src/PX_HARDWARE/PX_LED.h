/*
    *** Buttons, LEDs & Power related operations ***

          PX_BUTTON class:


          PX_LED class:


          PX_POWER class:
*/

#pragma once

// Preliminary
// LED
#include <Adafruit_NeoPixel.h>

// PHOTONIC DISCHARGE DEVICE aka LED
// LED strip
#define LED_FPS 2
#define LED_STRIP_PIN 4
#define LED_NUM_PIXELS 8
// bool toggleLED = false;
// Modes: -1 = No Mode, 0 = First Mode, 1 = Second Mode, ...
// #define BTN_1_LED_POSITION 0
// #define BTN_2_LED_POSITION 1
// temporary
#define BATTERY_FULL 3
#define BATTERY_HALF 2
#define BATTERY_LOW 1
#define BATTERY_CRITICAL 0

class PX_LED_element
{
private:
    int position = 0;
    int color[3] = {0, 0, 0};
    int brightness = 0;

public:
    PX_LED_element();
    PX_LED_element(int pos, int R, int G, int B, int brightness);

    int getPosition();
    void close();
};

class PX_LED_handler
{
private:
    PX_LED_element root_LED;
    PX_LED_element scale_LED;
    PX_LED_element chord_LED;
    PX_LED_element arp_LED;
    PX_LED_element on_off_LED;
    PX_LED_element toggle_mouse_LED;
    PX_LED_element solo_LED;
    PX_LED_element VG_LED;

    void setup();
    // Timers
    unsigned long LED_FPS_timer = millis(), LED_t0 = millis();

public:
    PX_LED_handler();
    PX_LED_handler(bool init);

    void loop();
    void showTargetLED(int targetLED,
                       int targetLEDColorR,
                       int targetLEDColorG,
                       int targetLEDColorB,
                       int targetLEDBrightness);

    void shineAllLEDs();
    void closeTargetLED(int targetLED);
    void closeAllLEDs();
    void batteryMonitor(int batteryLevel);
    void celebrate();
};