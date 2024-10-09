#include <PX_HARDWARE/PX_LED.h>

PX_LED_element::PX_LED_element() {}

PX_LED_element::PX_LED_element(int pos, int R, int G, int B, int brightness)
{
    position = pos;
    // setColor(R, G, B);
    // setBrightness(brightness);
}

int PX_LED_element::getPosition()
{
    return position;
}

void PX_LED_element::close()
{
    // LED_STRIP.setPixelColor(position, LED_STRIP.Color(0, 0, 0));
}

PX_LED_element root_LED;
PX_LED_element scale_LED;
PX_LED_element chord_LED;
PX_LED_element arp_LED;
PX_LED_element on_off_LED;
PX_LED_element toggle_mouse_LED;
PX_LED_element solo_LED;
PX_LED_element VG_LED;

void PX_LED_handler::setup()
{
    // solo_LED = PX_LED_element(BTN_1_LED_POSITION, 0.635 * 255, 0.000 * 255, 1.000 * 255, 15);
    // VG_LED = PX_LED_element(BTN_2_LED_POSITION, 0.992 * 255, 0.000 * 255, 1.000 * 255, 15);
}
// Timers
unsigned long LED_FPS_timer = millis(), LED_t0 = millis();

// PX_LED_handler::PX_LED_handler() : solo_LED(BTN_1_LED_POSITION, 0.635 * 255, 0.000 * 255, 1.000 * 255, 15),
//                                    VG_LED(BTN_2_LED_POSITION, 0.992 * 255, 0.000 * 255, 1.000 * 255, 15) {}

// BATTERY_LED(BTN_2_LED_POSITION, 0.992*255, 0.000*255, 1.000*255, 15){}

PX_LED_handler::PX_LED_handler(bool init)
{
    setup();
}

void PX_LED_handler::loop()
{
    // Update Frame every FPS ms
    if ((millis() - LED_FPS_timer) > LED_FPS)
    {
        // Clear canvas
        // LED_STRIP.clear();
        // Set individual LED colors
        // Show LEDs
        // LED_STRIP.show();
        // batteryMonitor(power.get_battery_level());
        LED_FPS_timer = millis();
    }
    // Close LEDs until time to open
    else
    {
        // LED_STRIP.clear();
        // LED_STRIP.show();
    }
}

void PX_LED_handler::showTargetLED(int targetLED,
                                   int targetLEDColorR,
                                   int targetLEDColorG,
                                   int targetLEDColorB,
                                   int targetLEDBrightness)
{
}

void PX_LED_handler::shineAllLEDs()
{
    for (int i = 0; i < 10; i++)
    {
    }
}

void PX_LED_handler::closeTargetLED(int targetLED)
{
}

void PX_LED_handler::closeAllLEDs()
{
}

void PX_LED_handler::batteryMonitor(int batteryLevel)
{
    switch (batteryLevel)
    {
    case BATTERY_FULL:
        showTargetLED(8, 0, 255, 0, 10);
        break;
    case BATTERY_HALF:
        showTargetLED(8, 0, 122, 122, 10);
        break;
    case BATTERY_LOW:
        showTargetLED(8, 85, 85, 85, 10);
        break;
    case BATTERY_CRITICAL:
        showTargetLED(8, 255, 0, 0, 10);
        break;
    default:
        closeTargetLED(8);
    }
}

void PX_LED_handler::celebrate()
{
    // Yoo!!
    shineAllLEDs();
    delay(100);
    closeAllLEDs();
    delay(100);
    shineAllLEDs();
    delay(100);
    closeAllLEDs();
    delay(100);
}