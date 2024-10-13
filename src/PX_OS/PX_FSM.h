#pragma once

// I2C
constexpr int I2C_SCL = 26;
constexpr int I2C_SDA = 27;
// Main DBG
constexpr bool monitorActive = false;
constexpr int monitorHz = 1000;

class PX_GYRO;
class PX_MIDI;

class PX_FSM
{
private:
	PX_GYRO *gyroDriver = nullptr;
	PX_MIDI *midiDriver = nullptr;
	// Timers
	unsigned long FSM_t0 = 0;

public:
	PX_FSM();
	void setDrivers(PX_GYRO *gyroDriver, PX_MIDI *midiDriver);

	void begin();
	void loop();
};