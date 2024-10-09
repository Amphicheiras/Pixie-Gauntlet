#pragma once

#define MIDI_CC 176

class PX_BLACKMAGIC;
class PX_GYRO;

class PX_MIDI
{
private:
	PX_BLACKMAGIC *blackmagic;
	PX_GYRO *gyroDriver;

	// MIDI state transmition (must be over BLACKMAGIC or undefined error)
	bool pitchTransmit{true}, rollTransmit{true}, yawTransmit{true};
	bool xTransmit{true}, yTransmit{true}, zTransmit{true};
	// Is connected to rtp session?
	bool isConnected{false};
	// YRP After degrees2MIDI
	float pitchMIDI{0}, rollMIDI{0}, yawMIDI{0};
	// XYZ After position2MIDI
	float xMIDI{0}, yMIDI{0}, zMIDI{0};
	// A MIDI Note buffer has 1 value
	int MIDINoteOn{10}, MIDINoteOff{0};
	// Timers
	unsigned long MIDI_t0;
	double Arp_1, Arp_2, Arp_3;
	bool controlsActive = true;
	// Arpeggiator
	bool arpFlag = false;

public:
	PX_MIDI();
	void setDrivers(PX_GYRO *gyroDriver, PX_BLACKMAGIC *blackmagic);
	void begin();
	// enableMIDIControl?
	void enableControl();
	void disableControl();
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
	bool getPitchTransmission();
	bool getRollTransmission();
	bool getYawTransmission();
	void setPitchTransmission(bool pitchTransmission);
	void setRollTransmission(bool rollTransmission);
	void setYawTransmission(bool yawTransmission);
	float getMIDIPitch();
	float getMIDIRoll();
	float getMIDIYaw();
};