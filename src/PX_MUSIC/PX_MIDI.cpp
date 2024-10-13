// Enterprise
#include <UF0_DBG.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <PX_HARDWARE/PX_GYRO.h>
#include <PX_OS/PX_BLACKMAGIC.h>
// Wi-Fi
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
// MIDI over Wi-Fi (rtpMIDI)
#include <AppleMIDI.h>

APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

unsigned long MIDI_t0 = millis();
double Arp_1, Arp_2, Arp_3 = millis();

PX_MIDI::PX_MIDI() {}

void PX_MIDI::setDrivers(PX_GYRO *gyroDriver, PX_BLACKMAGIC *blackmagic)
{
	this->gyroDriver = gyroDriver;
	this->blackmagic = blackmagic;
}

void PX_MIDI::begin()
{
	DBG(F("rtpMIDI RRrruUULLEeezzZZZzz!!"));
	DBG(F("Add rtpMIDI device named Arduino with IP:"));
	DBG(WiFi.localIP());
	DBG(F("@ Port"), AppleMIDI.getPort());

	MIDI.begin();

	AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t &ssrc, const char *name)
								 {
        // isConnected = true;
        DBG(F("Connected to rtpMIDI session with"), name); });

	AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t &ssrc)
									{
       	// isConnected = false;
       	DBG(F("Disconnected from rtpMIDI session")); });

	// // // Lambda functions?
	//   // MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
	//   //   DBG(F("NoteOn"), note);
	//   // });
}

// enableMIDIControl?
void PX_MIDI::enableControl()
{
	controlsActive = true;
}

void PX_MIDI::disableControl()
{
	controlsActive = false;
}

void PX_MIDI::enableDelay()
{
	sendMIDIControlChange(MIDI_CC, 127, 7);
	delayActive = true;
}

void PX_MIDI::disableDelay()
{
	sendMIDIControlChange(MIDI_CC, 0, 7);
	delayActive = false;
}

bool PX_MIDI::getControlsActive()
{
	return controlsActive;
}

bool PX_MIDI::getDelayActive()
{
	return delayActive;
}

// Drum hit gesture
void PX_MIDI::drumHitMIDI()
{
	// If drum is hit
	if (gyroDriver->drumHit() > 0)
	{
		// Toggle MIDI Note on drum hit
		if (!isPlaying)
		{
			sendMIDINoteOn(10, 100, 5);
		}
		else
		{
			sendMIDINoteOff(10, 100, 5);
		}
	}
}

void PX_MIDI::loop()
{
	// Listen to incoming notes
	MIDI.read();
	// drumHitMIDI();
	if (controlsActive)
	{
		sendControls();
	}
	drumHitMIDI();
}

void PX_MIDI::sendControls()
{
	// Transmit YPR
	if (pitchTransmit)
	{
		pitchMIDI = degrees2MIDI(gyroDriver->getPitch(), -90, 90);
		MIDI.sendControlChange(MIDI_CC, pitchMIDI, 1);
	}
	if (rollTransmit)
	{
		rollMIDI = degrees2MIDI(gyroDriver->getRoll(), -90, 90, false, true);
		MIDI.sendControlChange(MIDI_CC, rollMIDI, 2);
	}
	if (yawTransmit)
	{
		yawMIDI = degrees2MIDI(gyroDriver->getYaw(), -90, 90, true);
		MIDI.sendControlChange(MIDI_CC, yawMIDI, 3);
	}
	// Transmit XYZ
	if (xTransmit)
	{
		accelerationXMIDI = acceleration2MIDI(gyroDriver->getAccelerationX(), -10, 10);
		MIDI.sendControlChange(MIDI_CC, accelerationXMIDI, 4);
	}
	if (yTransmit)
	{
		accelerationYMIDI = acceleration2MIDI(gyroDriver->getAccelerationY(), -10, 10, true);
		MIDI.sendControlChange(MIDI_CC, accelerationYMIDI, 5);
	}
	if (zTransmit)
	{
		accelerationZMIDI = acceleration2MIDI(gyroDriver->getAccelerationZ(), -10, 10);
		MIDI.sendControlChange(MIDI_CC, accelerationZMIDI, 6);
	}
}
// Send toggle on from trigger 3
void PX_MIDI::toggle_T3_on()
{
	MIDI.sendControlChange(MIDI_CC, 127, 15);
}
// Send toggle off from trigger 3
void PX_MIDI::toggle_T3_off()
{
	MIDI.sendControlChange(MIDI_CC, 0, 15);
}

// MIDI transmission control (mostly from website)
// Enable MIDI transmission for toStart parameter
void PX_MIDI::startTransmission(bool *toStart)
{
	*toStart = true;
}
// Disable MIDI transmission for toStart parameter
void PX_MIDI::stopTransmission(bool *toStop)
{
	*toStop = false;
}

void PX_MIDI::startPitchTransmission()
{
	startTransmission(&pitchTransmit);
}

void PX_MIDI::stopPitchTransmission()
{
	stopTransmission(&pitchTransmit);
}

void PX_MIDI::startRollTransmission()
{
	startTransmission(&pitchTransmit);
}

void PX_MIDI::stopRollTransmission()
{
	stopTransmission(&rollTransmit);
}

void PX_MIDI::startYawTransmission()
{
	startTransmission(&rollTransmit);
}

void PX_MIDI::stopYawTransmission()
{
	stopTransmission(&yawTransmit);
}

// Enable all MIDI transmissions
void PX_MIDI::startTransmissionAll()
{
	startTransmission(&pitchTransmit);
	startTransmission(&rollTransmit);
	startTransmission(&yawTransmit);
	startTransmission(&xTransmit);
	startTransmission(&yTransmit);
	startTransmission(&zTransmit);
}
// Disable all MIDI transmissions
void PX_MIDI::stopTransmissionAll()
{
	stopTransmission(&pitchTransmit);
	stopTransmission(&rollTransmit);
	stopTransmission(&yawTransmit);
	stopTransmission(&xTransmit);
	stopTransmission(&yTransmit);
	stopTransmission(&zTransmit);
}
// Solo MIDI transmission for toSolo parameter
void PX_MIDI::soloTransmission(bool *toSolo)
{
	stopTransmissionAll();
	// *toSolo = true;
	startTransmission(toSolo);
}
// Send MIDI note on
void PX_MIDI::sendMIDINoteOn(int note, int velocity, int channel)
{
	MIDI.sendNoteOn(note, velocity, channel);
	isPlaying = true;
}
// Send MIDI note off
void PX_MIDI::sendMIDINoteOff(int note, int velocity, int channel)
{
	MIDI.sendNoteOff(note, velocity, channel);
	isPlaying = false;
}
bool PX_MIDI::getIsPlaying()
{
	return isPlaying;
}
// Send MIDI note on
void PX_MIDI::sendMIDIChordOn(int note, int velocity, int channel)
{
	MIDI.sendNoteOn(note, velocity, channel);
	MIDI.sendNoteOn(note, velocity, channel);
	MIDI.sendNoteOn(note + 3, velocity, channel);
	MIDI.sendNoteOn(note + 5, velocity, channel);
}
// Send MIDI note off
void PX_MIDI::sendMIDIChordOff(int note, int velocity, int channel)
{
	MIDI.sendNoteOff(note, velocity, channel);
	MIDI.sendNoteOff(note, velocity, channel);
	MIDI.sendNoteOff(note + 3, velocity, channel);
	MIDI.sendNoteOff(note + 5, velocity, channel);
}
// tilt: Gyro input, fromAngle: lowerLimit, toAngle: higherLimit, shift: move angle by 'shift', invert: invert tilt direction
double PX_MIDI::degrees2MIDI(double tilt, int fromAngle, int toAngle, bool shift, bool invert)
{
	// Map [0, 360] -> [-180, 180]
	if (shift)
	{
		if (tilt > 180)
			tilt = map(tilt, 360, 181, -1, -180);
	}
	// Return if out of bounds
	if (tilt < fromAngle)
		return 0;
	if (tilt > toAngle)
		return 127;
	// Map [-fromAngle, toAngle] -> [0, 127]
	if (tilt < 0)
		tilt = map(tilt, 0, fromAngle, 64, 0);
	else
		tilt = map(tilt, 0, toAngle, 64, 127);
	// Invert movement direction
	if (invert)
		return (127 - tilt);
	else
		return tilt;
}
// position: Gyro input, fromPosition: lowerLimit, toPosition: higherLimit, invert: invert position direction
double PX_MIDI::acceleration2MIDI(double acceleration, int fromAcceleration, int toAcceleration, bool invert)
{
	// Return if out of bounds
	if (acceleration < fromAcceleration)
	{
		return 0;
	}
	if (acceleration > toAcceleration)
	{
		return 127;
	}
	// Map [-fromAngle, toAngle] -> [0, 127]
	if (acceleration < 0)
	{
		acceleration = map(acceleration, 0, fromAcceleration, 64, 0);
	}
	else
	{
		acceleration = map(acceleration, 0, toAcceleration, 64, 127);
	}
	// Invert movement direction
	if (invert)
	{
		return (127 - acceleration);
	}
	else
	{
		return acceleration;
	}
}
// position: Gyro input, fromPosition: lowerLimit, toPosition: higherLimit, invert: invert position direction
double PX_MIDI::position2MIDI(double position, int fromPosition, int toPosition, bool invert)
{
	// Return if out of bounds
	if (position < fromPosition)
	{
		return 0;
	}
	if (position > toPosition)
	{
		return 127;
	}
	// Map [-fromAngle, toAngle] -> [0, 127]
	if (position < 0)
	{
		position = map(position, 0, fromPosition, 64, 0);
	}
	else
	{
		position = map(position, 0, toPosition, 64, 127);
	}
	// Invert movement direction
	if (invert)
	{
		return (127 - position);
	}
	else
	{
		return position;
	}
}

bool PX_MIDI::getPitchTransmission()
{
	return pitchTransmit;
}

bool PX_MIDI::getRollTransmission()
{
	return rollTransmit;
}

bool PX_MIDI::getYawTransmission()
{
	return yawTransmit;
}

void PX_MIDI::soloPitchTransmission()
{
	soloTransmission(&pitchTransmit);
}

void PX_MIDI::soloRollTransmission()
{
	soloTransmission(&rollTransmit);
}

void PX_MIDI::soloYawTransmission()
{
	soloTransmission(&yawTransmit);
}

void PX_MIDI::setPitchTransmission(bool pitchTransmission)
{
	this->pitchTransmit = pitchTransmission;
}

void PX_MIDI::setRollTransmission(bool rollTransmission)
{
	this->rollTransmit = rollTransmission;
}

void PX_MIDI::setYawTransmission(bool yawTransmission)
{
	this->yawTransmit = yawTransmission;
}

void PX_MIDI::sendMIDIControlChange(int CC, int velocity, int channel)
{
	MIDI.sendControlChange(CC, velocity, channel);
}

float PX_MIDI::getMIDIPitch()
{
	return pitchMIDI;
}

float PX_MIDI::getMIDIRoll()
{
	return rollMIDI;
}

float PX_MIDI::getMIDIYaw()
{
	return yawMIDI;
}

float PX_MIDI::getMIDIAccelerationX()
{
	return accelerationXMIDI;
}

float PX_MIDI::getMIDIAccelerationY()
{
	return accelerationYMIDI;
}

float PX_MIDI::getMIDIAccelerationZ()
{
	return accelerationZMIDI;
}

void PX_MIDI::setAccelXTransmission(bool accelXTransmission)
{
	this->xTransmit = accelXTransmission;
}

void PX_MIDI::setAccelYTransmission(bool accelYTransmission)
{
	this->yTransmit = accelYTransmission;
}

void PX_MIDI::setAccelZTransmission(bool accelZTransmission)
{
	this->zTransmit = accelZTransmission;
}

bool PX_MIDI::getAccelXTransmission()
{
	return xTransmit;
}

bool PX_MIDI::getAccelYTransmission()
{
	return yTransmit;
}

bool PX_MIDI::getAccelZTransmission()
{
	return zTransmit;
}