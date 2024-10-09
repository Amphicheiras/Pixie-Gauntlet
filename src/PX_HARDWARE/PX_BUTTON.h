/*
    *** Buttons, LEDs & Power related operations ***

          PX_BUTTON class:


          PX_LED class:


          PX_POWER class:
*/

#pragma once

// BUTTON functions position
#define BTN_1_POSITION 0 // R
#define BTN_2_POSITION 1 // G
#define BTN_3_POSITION 2 // B
#define LED_R 15
#define LED_G 5
#define LED_B 22
// BUTTON modeZ
#define NO_MODE -1
#define SOLO_MODE 0
#define BTN_1_LED_POSITION BTN_1_POSITION
#define BTN_2_LED_POSITION BTN_2_POSITION

class PX_MIDI;

class PX_BUTTON
{
private:
    PX_MIDI *midiDriver = nullptr;

    // BUTTON position pinZ
    int BUTTON_POSITION_PIN[3] = {2, 18, 23};
    // BUTTON functions position pinZ
    int BTN_1_PIN = BUTTON_POSITION_PIN[BTN_1_POSITION];
    int BTN_2_PIN = BUTTON_POSITION_PIN[BTN_2_POSITION];
    int BTN_3_PIN = BUTTON_POSITION_PIN[BTN_3_POSITION];
    // BUTTON current states & past states
    bool BTN_1_STATE = false;
    bool BTN_1_STATE_past = false;
    bool BTN_2_STATE = false;
    bool BTN_2_STATE_past = false;
    bool BTN_3_STATE = false;
    bool BTN_3_STATE_past = false;
    // Button toggle flags
    bool toggleFlag_1 = false;
    bool toggleFlag_2 = false;
    bool toggleFlag_3 = false;
    int debounceTime = 30;
    // Modes: -1 = No Mode, 0 = First Mode, 1 = Second Mode, ...
    int btnMode = NO_MODE; // modes must go to PX_FSM

public:
    PX_BUTTON();

    void setDrivers(PX_MIDI *midiDriver);
    void begin();
    void loop();
};