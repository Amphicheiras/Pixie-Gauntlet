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
#define BATTERY_GAUGE_PIN 34
//

class PX_POWER
{
    // 0-100%: 4h45m 3200mAh
private:
    float raw_voltage;
    int supply_voltage, voltage_percent, voltageNormalized, voltageSampleBuffer[10];
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