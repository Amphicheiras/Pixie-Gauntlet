#include <PX_HARDWARE/PX_BUTTON.h>
#include <UF0_DBG.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>

PX_BUTTON::PX_BUTTON()
{
    BTN_1_STATE = false;
}

void PX_BUTTON::setDrivers(PX_MIDI *midiDriver)
{
    this->midiDriver = midiDriver;
}

void PX_BUTTON::begin()
{
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
    // Check if any MOD buttons are pressed
    switch (btnMode)
    {
        // BTN do:
        //////////////// N O    M O D E //////////////
    case NO_MODE:
        // // Hold T1:
        // if (BTN_1_STATE == HIGH && BTN_1_STATE_past == LOW)
        // {
        // 	// Turn off previously played note, if any
        // 	midii->sendMIDINoteOn(MIDINoteOn, 100, 5);
        // 	// midii->sendMIDIChordOn(MIDINoteOn, 70, 5);
        // }
        // // Let T1:
        // if (BTN_1_STATE == LOW && BTN_1_STATE_past == HIGH)
        // {
        // 	// Turn off previously played note, if any
        // 	midii->sendMIDINoteOff(MIDINoteOn, 100, 5);
        // 	// midii->sendMIDIChordOff(MIDINoteOn, 70, 5);
        // }
        // Toggle B1:
        if (BTN_1_STATE == HIGH && BTN_1_STATE_past == LOW)
        {
            DBG("Button #1 =", toggleFlag_1);
            toggleFlag_1 = !toggleFlag_1;
            if (toggleFlag_1)
            {
                // Turn off previously played note, if any
                midiDriver->sendMIDINoteOn(10, 100, 5);
                // midii->sendMIDIChordOn(MIDINoteOn, 70, 5);
                analogWrite(LED_R, 80);
            }
            else
            {
                // Turn off previously played note, if any
                midiDriver->sendMIDINoteOff(10, 100, 5);
                // midii->sendMIDIChordOff(MIDINoteOn, 70, 5);
                analogWrite(LED_R, LOW);
            }
        }
        // Toggle B2:
        if (BTN_2_STATE == HIGH && BTN_2_STATE_past == LOW)
        {
            DBG("Button #2 =", toggleFlag_2);
            toggleFlag_2 = !toggleFlag_2;
            if (toggleFlag_2)
            {
                midiDriver->enableControl();
                analogWrite(LED_G, 80);
            }
            else
            {
                midiDriver->disableControl();
                analogWrite(LED_G, LOW);
            }
        }
        // Toggle B3:
        if (BTN_3_STATE == HIGH && BTN_3_STATE_past == LOW)
        {
            DBG("Button #3 =", toggleFlag_3);
            toggleFlag_3 = !toggleFlag_3;
            if (toggleFlag_3)
            {
                midiDriver->sendMIDIControlChange(MIDI_CC, 127, 7);
                analogWrite(LED_B, 80);
            }
            else
            {
                midiDriver->sendMIDIControlChange(MIDI_CC, 0, 7);
                analogWrite(LED_B, LOW);
            }
        }
        break;
    //////////////// S O L O   M O D E ///////////////
    case SOLO_MODE:
        if (BTN_1_STATE == HIGH && BTN_1_STATE_past == LOW)
        {
        }
        if (BTN_2_STATE == HIGH && BTN_2_STATE_past == LOW)
        {
        }
        if (BTN_3_STATE == HIGH && BTN_3_STATE_past == LOW)
        {
        }
        break;
    default:
    }

    // Update old BTN states
    BTN_1_STATE_past = BTN_1_STATE;
    BTN_2_STATE_past = BTN_2_STATE;
    BTN_3_STATE_past = BTN_3_STATE;
}