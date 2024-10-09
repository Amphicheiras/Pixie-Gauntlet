#pragma once

class PX_GYRO;
class PX_MIDI;

class PX_BLACKMAGIC
{
private:
	// Virtual Grid current target
	int virtualTarget;
	PX_GYRO *gyroDriver;
	PX_MIDI *midiDriver;

	// pop_XY = population XY, Mmin_XY = XY min movement, Mmax_XY = XY max movement
	int virtualGrid(int pop_X, int pop_Y, float Mmin_X, float Mmax_X, float Mmin_Y, float Mmax_Y);

public:
	PX_BLACKMAGIC();

	void setDrivers(PX_GYRO *gyroDriver, PX_MIDI *midiDriver);
	void begin();
	void loop();
	int getVirtualTarget() const;
	void savePreset();
	void loadPreset();
};