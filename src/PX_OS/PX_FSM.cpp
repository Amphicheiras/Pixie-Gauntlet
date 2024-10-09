#include <PX_OS/PX_FSM.h>
// Enterprise
#include <UF0_DBG.h>
#include "PX_MUSIC/PX_MIDI.h"
#include "PX_HARDWARE/PX_GYRO.h"
#include <Wire.h>
// Disk access
#include <Preferences.h>

PX_FSM::PX_FSM() {}

void PX_FSM::setDrivers(PX_GYRO *gyroDriver, PX_MIDI *midiDriver)
{
	this->gyroDriver = gyroDriver;
	this->midiDriver = midiDriver;
}

void PX_FSM::begin()
{
	FSM_t0 = millis();
}

void PX_FSM::loop()
{
	if (monitorActive && (millis() - FSM_t0) > monitorHz)
	{
		DBG(F("\nDegrees Roll:"), gyroDriver->getRoll(), F("\tMIDI Roll:"), midiDriver->getMIDIRoll());
		DBG(F("Degrees Pitch:"), gyroDriver->getPitch(), F("\tMIDI Pitch:"), midiDriver->getMIDIPitch());
		DBG(F("Degrees Yaw:"), gyroDriver->getYaw(), F("\tMIDI Yaw:"), midiDriver->getMIDIYaw());
		DBG(F("State Roll, Pitch, Yaw:"), midiDriver->getRollTransmission(), midiDriver->getPitchTransmission(), midiDriver->getYawTransmission());
		// DBG(F("Raw X:"), xPosition, F("\tMIDI X:"), xMIDI);
		// DBG(F("Raw Y:"), yPosition, F("\tMIDI Y:"), yMIDI);
		// DBG(F("Raw Z:"), zPosition, F("\tMIDI Z:"), zMIDI);
		// DBG(F("State X, Y, Z:"), xTransmit, yTransmit, zTransmit);
		// DBG(F("Virtual Target:"), virtualTarget);
		// DBG(F("IP:"), WiFi.localIP());
		FSM_t0 = millis();
	}
}