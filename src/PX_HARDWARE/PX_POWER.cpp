#include <PX_HARDWARE/PX_POWER.h>

float raw_voltage;
int supply_voltage, voltage_percent, voltageNormalized, voltageSampleBuffer[10];
// Timers
unsigned long POWER_t0 = millis();

void PX_POWER::begin()
{
    measure_battery_level();
}

void PX_POWER::loop()
{
    
}

void PX_POWER::measure_battery_level()
{
    raw_voltage = (analogRead(BATTERY_GAUGE_PIN) * 3.3) / 4095;
    supply_voltage = 2 * 1000 * ((analogRead(BATTERY_GAUGE_PIN) * 3.3) / 4095) + 0.2;
    voltage_percent = map(supply_voltage, 2900, 4200, 0, 100);
}

PX_POWER::PX_POWER() {}

int PX_POWER::get_battery_percent()
{
    measure_battery_level();
    return voltage_percent;
}

int PX_POWER::get_battery_level()
{
    // Average battery level values (sum(10*samples)/10)
    // for (t0 - millis() > 10){
    //   voltageSampleBuffer[i] = voltage_percent;
    //   i++;
    //   if (i > 9){
    //     voltageNormalized = voltageSampleBuffer.sum()/10;
    //     i=0;
    //   }
    //   t0 = millis();
    // }

    switch (get_battery_percent())
    {
    case 61 ... 100:
        return BATTERY_FULL;
    case 31 ... 60:
        return BATTERY_HALF;
    case 11 ... 30:
        return BATTERY_LOW;
    case 0 ... 10:
        return BATTERY_CRITICAL;
    default:
        return -1;
    }
}