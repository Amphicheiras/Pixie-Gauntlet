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
		DBG(F("\nDegrees Pitch:"), gyroDriver->getPitch(), F("\tMIDI Pitch:"), midiDriver->getMIDIPitch());
		DBG(F("Degrees Roll:"), gyroDriver->getRoll(), F("\tMIDI Roll:"), midiDriver->getMIDIRoll());
		DBG(F("Degrees Yaw:"), gyroDriver->getYaw(), F("\tMIDI Yaw:"), midiDriver->getMIDIYaw());
		DBG(F("State Pitch, Roll, Yaw:"), midiDriver->getPitchTransmission(), midiDriver->getRollTransmission(), midiDriver->getYawTransmission());
		DBG(F("Acceleration X:"), gyroDriver->getAccelerationX(), F("\tMIDI Acceleration X:"), midiDriver->getMIDIAccelerationX());
		DBG(F("Acceleration Y:"), gyroDriver->getAccelerationY(), F("\tMIDI Acceleration Y:"), midiDriver->getMIDIAccelerationY());
		DBG(F("Acceleration Z:"), gyroDriver->getAccelerationZ(), F("\tMIDI Acceleration Z:"), midiDriver->getMIDIAccelerationZ());
		DBG(F("State X, Y, Z:"), midiDriver->getAccelXTransmission(), midiDriver->getAccelYTransmission(), midiDriver->getAccelZTransmission());
		FSM_t0 = millis();
	}
}