#pragma once

// I2C
constexpr int I2C_SCL = 26;
constexpr int I2C_SDA = 27;
// Main DBG
constexpr bool monitorActive = true;
constexpr int monitorHz = 600;

class PX_GYRO;
class PX_MIDI;

class PX_FSM
{
private:
	PX_GYRO *gyroDriver;
	PX_MIDI *midiDriver;
	// Timers
	unsigned long FSM_t0;

public:
	PX_FSM();
	void setDrivers(PX_GYRO *gyroDriver, PX_MIDI *midiDriver);

	void begin();
	void loop();
};