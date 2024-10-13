#include <PX_HARDWARE/PX_BUTTON.h>
#include <UF0_DBG.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

PX_BUTTON::PX_BUTTON() {}

void PX_BUTTON::setDrivers(PX_MIDI *midiDriver)
{
    this->midiDriver = midiDriver;
}

void PX_BUTTON::begin()
{
    timer_led_1 = millis();
    timer_led_2 = millis();
    timer_led_3 = millis();
    timer_led_33 = millis();

    pinMode(BTN_1_PIN, INPUT_PULLUP);
    pinMode(BTN_2_PIN, INPUT_PULLUP);
    pinMode(BTN_3_PIN, INPUT_PULLUP);

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
}

void PX_BUTTON::loop()
{
    // Update BTN states
    BTN_1_STATE = !digitalRead(BTN_1_PIN);
    BTN_2_STATE = !digitalRead(BTN_2_PIN);
    BTN_3_STATE = !digitalRead(BTN_3_PIN);

    // Check modes
    switch (btnMode)
    {
    //////////////// NO  M O D E ///////////////
    case NO_MODE:
        // Toggle B1:
        if (BTN_1_STATE == LOW && BTN_1_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #1 =", toggleFlag_1);
            toggleFlag_1 = !toggleFlag_1;
            if (toggleFlag_1)
            {
                // Turn off previously played note, if any
                midiDriver->sendMIDINoteOn(10, 100, 5);
            }
            else
            {
                // Turn off previously played note, if any
                midiDriver->sendMIDINoteOff(10, 100, 5);
                analogWrite(LED_R, LED_OFF);
            }
        }
        // Toggle B2:
        if (BTN_2_STATE == LOW && BTN_2_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #2 =", toggleFlag_2);
            toggleFlag_2 = !toggleFlag_2;
            if (toggleFlag_2)
            {
                midiDriver->enableControl();
            }
            else
            {
                midiDriver->disableControl();
            }
        }
        // Toggle B3:
        if (BTN_3_STATE == LOW && BTN_3_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #3 =", toggleFlag_3);
            toggleFlag_3 = !toggleFlag_3;
            if (toggleFlag_3)
            {
                midiDriver->enableDelay();
            }
            else
            {
                midiDriver->disableDelay();
            }
        }
        if ((millis() - timer_led_1) > 800)
        {
            blink_counter_1 = 0;
            timer_led_1 = millis();
        }
        if ((blink_counter_1 < 5) && (millis() - timer_led_11) > 100)
        {
            led_state_1 = !led_state_1;
            timer_led_11 = millis();
            blink_counter_1++;
        }
        if (blink_counter_1 >= 5)
        {
            led_state_1 = midiDriver->getIsPlaying();
        }
        analogWrite(LED_R, led_state_1 ? LED_ON : LED_OFF);
        analogWrite(LED_G, midiDriver->getControlsActive() ? LED_ON : LED_OFF);
        analogWrite(LED_B, midiDriver->getDelayActive() ? LED_ON : LED_OFF);
        break;
    //////////////// Y P R  M O D E ///////////////
    case YPR_MODE:
        // Toggle B1:
        if (BTN_1_STATE == LOW && BTN_1_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #4 =", toggleFlag_4);
            toggleFlag_4 = !toggleFlag_4;
            midiDriver->setPitchTransmission(!midiDriver->getPitchTransmission());
        }
        // Toggle B2:
        if (BTN_2_STATE == LOW && BTN_2_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #5 =", toggleFlag_5);
            toggleFlag_5 = !toggleFlag_5;
            midiDriver->setRollTransmission(!midiDriver->getRollTransmission());
        }
        // Toggle B3:
        if (BTN_3_STATE == LOW && BTN_3_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #6 =", toggleFlag_6);
            toggleFlag_6 = !toggleFlag_6;
            midiDriver->setYawTransmission(!midiDriver->getYawTransmission());
        }
        analogWrite(LED_R, midiDriver->getPitchTransmission() ? LED_ON : LED_OFF);
        if ((millis() - timer_led_2) > 800)
        {
            blink_counter_2 = 0;
            timer_led_2 = millis();
        }
        if ((blink_counter_2 < 5) && (millis() - timer_led_22) > 100)
        {
            led_state_2 = !led_state_2;
            timer_led_22 = millis();
            blink_counter_2++;
        }
        if (blink_counter_2 >= 5)
        {
            led_state_2 = midiDriver->getRollTransmission();
        }
        analogWrite(LED_G, led_state_2 ? LED_ON : LED_OFF);
        analogWrite(LED_B, midiDriver->getYawTransmission() ? LED_ON : LED_OFF);
        break;
    //////////////// X Y Z  M O D E ///////////////
    case XYZ_MODE:
        // Toggle B1:
        if (BTN_1_STATE == LOW && BTN_1_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #7 =", toggleFlag_7);
            toggleFlag_7 = !toggleFlag_7;
            midiDriver->setAccelXTransmission(!midiDriver->getAccelXTransmission());
        }
        // Toggle B2:
        if (BTN_2_STATE == LOW && BTN_2_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #8 =", toggleFlag_8);
            toggleFlag_8 = !toggleFlag_8;
            midiDriver->setAccelYTransmission(!midiDriver->getAccelYTransmission());
        }
        // Toggle B3:
        if (BTN_3_STATE == LOW && BTN_3_STATE_past == HIGH && modeFlag)
        {
            DBG("Button #9 =", toggleFlag_9);
            toggleFlag_9 = !toggleFlag_9;
            midiDriver->setAccelZTransmission(!midiDriver->getAccelZTransmission());
        }
        analogWrite(LED_R, midiDriver->getAccelXTransmission() ? LED_ON : LED_OFF);
        analogWrite(LED_G, midiDriver->getAccelYTransmission() ? LED_ON : LED_OFF);
        if ((millis() - timer_led_3) > 800)
        {
            blink_counter_3 = 0;
            timer_led_3 = millis();
        }
        if ((blink_counter_3 < 5) && (millis() - timer_led_33) > 100)
        {
            led_state_3 = !led_state_3;
            timer_led_33 = millis();
            blink_counter_3++;
        }
        if (blink_counter_3 >= 5)
        {
            led_state_3 = midiDriver->getAccelZTransmission();
        }
        analogWrite(LED_B, led_state_3 ? LED_ON : LED_OFF);
        break;
    default:
        break;
    }

    // Check if mode button is pressed
    if (BTN_1_STATE == HIGH && BTN_2_STATE == HIGH && BTN_3_STATE == HIGH)
    {
        if (modeFlag)
        {
            // Cycle through the modes
            btnMode = (btnMode + 1) % TOTAL_MODES;

            // Debugging to see mode changes
            Serial.print("Switched to mode: ");
            Serial.println(btnMode);
        }
        modeFlag = false;
    }
    else if (BTN_1_STATE == LOW && BTN_2_STATE == LOW && BTN_3_STATE == LOW)
    {
        modeFlag = true;
    }

    // Update old BTN states
    BTN_1_STATE_past = BTN_1_STATE;
    BTN_2_STATE_past = BTN_2_STATE;
    BTN_3_STATE_past = BTN_3_STATE;
}