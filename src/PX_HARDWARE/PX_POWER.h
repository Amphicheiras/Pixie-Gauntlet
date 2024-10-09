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
#include <ezButton.h>
#define BATTERY_FULL 3
#define BATTERY_HALF 2
#define BATTERY_LOW 1
#define BATTERY_CRITICAL 0

// POWER SUPPLY
#define BATTERY_GAUGE_PIN 33
//

class PX_POWER
{
    // 0-100%: 4h45m 3200mAh
private:
    float raw_voltage = 0.0f;
    int supply_voltage = 0;
    float voltage_percent = 0.0f;
    float voltageNormalized = 0.0f;
    float voltageSampleBuffer[10] = {0.0f};
    // Timers
    unsigned long POWER_t0 = millis();

    void measure_battery_level();

public:
    PX_POWER();

    void begin();
    void loop();
    int get_battery_percent();
    int get_battery_level();
};