#pragma once

#define MIDI_CC 176

class PX_BLACKMAGIC;
class PX_GYRO;

class PX_MIDI
{
private:
	PX_BLACKMAGIC *blackmagic = nullptr;
	PX_GYRO *gyroDriver = nullptr;

	// MIDI state transmition (must be over BLACKMAGIC or undefined error)
	bool pitchTransmit{true}, rollTransmit{true}, yawTransmit{true};
	bool xTransmit{true}, yTransmit{true}, zTransmit{true};
	// Is connected to rtp session?
	bool isConnected{false};
	// YRP After degrees2MIDI
	float pitchMIDI{0}, rollMIDI{0}, yawMIDI{0};
	// XYZ After acceleration2MIDI
	float accelerationXMIDI{0}, accelerationYMIDI{0}, accelerationZMIDI{0};
	// XYZ After position2MIDI
	float xMIDI{0}, yMIDI{0}, zMIDI{0};
	// A MIDI Note buffer has 1 value
	int MIDINoteOn{10}, MIDINoteOff{0};
	// Timers
	unsigned long MIDI_t0 = 0;
	double Arp_1 = 0.0;
	double Arp_2 = 0.0;
	double Arp_3 = 0.0;
	bool controlsActive = false;
	bool delayActive = false;
	// Arpeggiator
	bool arpFlag = false;
	bool isPlaying = false;

public:
	PX_MIDI();
	void setDrivers(PX_GYRO *gyroDriver, PX_BLACKMAGIC *blackmagic);
	void begin();
	// enableMIDIControl?
	void enableControl();
	void disableControl();
	void enableDelay();
	void disableDelay();
	bool getControlsActive();
	bool getDelayActive();
	// Drum hit gesture
	void drumHitMIDI();
	void loop();
	void sendControls();
	// Send toggle on from trigger 3
	void toggle_T3_on();
	// Send toggle off from trigger 3;
	void toggle_T3_off();
	// MIDI transmission control (mostly from website)
	// Enable MIDI transmission for toStart parameter
	void startTransmission(bool *toStart);
	// Disable MIDI transmission for toStart parameter
	void stopTransmission(bool *toStop);
	void startPitchTransmission();
	void stopPitchTransmission();
	void startRollTransmission();
	void stopRollTransmission();
	void startYawTransmission();
	void stopYawTransmission();
	void soloPitchTransmission();
	void soloRollTransmission();
	void soloYawTransmission();
	// Enable all MIDI transmissions
	void startTransmissionAll();
	// Disable all MIDI transmissions
	void stopTransmissionAll();
	// Solo MIDI transmission for toSolo parameter
	void soloTransmission(bool *toSolo);
	// Send MIDI control change
	void sendMIDIControlChange(int CC, int velocity, int channel);
	// Send MIDI note on
	void sendMIDINoteOn(int note, int velocity, int channel);
	// Send MIDI note off
	void sendMIDINoteOff(int note, int velocity, int channel);
	// Send MIDI note on
	void sendMIDIChordOn(int note, int velocity, int channel);
	// Send MIDI note off
	void sendMIDIChordOff(int note, int velocity, int channel);
	// tilt: Gyro input, fromAngle: lowerLimit, toAngle: higherLimit, shift: move angle by 'shift', invert: invert tilt direction
	double degrees2MIDI(double tilt, int fromAngle, int toAngle, bool shift = false, bool invert = false);
	// position: Gyro input, fromPosition: lowerLimit, toPosition: higherLimit, invert: invert position direction
	double position2MIDI(double position, int fromPosition, int toPosition, bool invert);
	// position: Gyro input, fromPosition: lowerLimit, toPosition: higherLimit, invert: invert position direction
	double acceleration2MIDI(double acceleration, int fromAcceleration, int toAcceleration, bool invert = false);
	void setPitchTransmission(bool pitchTransmission);
	void setRollTransmission(bool rollTransmission);
	void setYawTransmission(bool yawTransmission);
	bool getPitchTransmission();
	bool getRollTransmission();
	bool getYawTransmission();
	void setAccelXTransmission(bool accelXTransmission);
	void setAccelYTransmission(bool accelYTransmission);
	void setAccelZTransmission(bool accelZTransmission);
	bool getAccelXTransmission();
	bool getAccelYTransmission();
	bool getAccelZTransmission();
	float getMIDIPitch();
	float getMIDIRoll();
	float getMIDIYaw();
	float getMIDIAccelerationX();
	float getMIDIAccelerationY();
	float getMIDIAccelerationZ();
	bool getIsPlaying();
};