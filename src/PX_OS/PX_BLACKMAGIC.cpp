// Enterprise
#include <PX_OS/PX_BLACKMAGIC.h>
#include <PX_HARDWARE/PX_GYRO.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <Preferences.h>

Preferences preferences;

PX_BLACKMAGIC::PX_BLACKMAGIC() {}

void PX_BLACKMAGIC::setDrivers(PX_GYRO *gyroDriver, PX_MIDI *midiDriver)
{
	this->gyroDriver = gyroDriver;
	this->midiDriver = midiDriver;
}

void PX_BLACKMAGIC::begin()
{
	loadPreset();
}

void PX_BLACKMAGIC::loop()
{
	virtualTarget = virtualGrid(4, 3, -90.0f, +90.0f, -90.0f, +90.0f);
}

// pop_XY = population XY, Mmin_XY = XY min movement, Mmax_XY = XY max movement
int PX_BLACKMAGIC::virtualGrid(int pop_X, int pop_Y, float Mmin_X, float Mmax_X, float Mmin_Y, float Mmax_Y)
{
	// DoF = Degrees of Freedom (Movement)
	float DoF_X = Mmax_X - Mmin_X;
	float DoF_Y = Mmax_Y - Mmin_Y;
	// 1 tick is 1 cell step
	float tick_X = DoF_X / pop_X;
	float tick_Y = DoF_Y / pop_Y;
	for (int i = 0; i < pop_X; i++)
	{
		// DBG("  YAW:", yaw , ">=", (i * tick_X), "&&", yaw, "<", ((i+1) * tick_X));
		if ((gyroDriver->getYaw() >= (i * tick_X + Mmin_X)) && (gyroDriver->getYaw() < ((i + 1) * tick_X + Mmin_X)))
		{
			for (int j = 0; j < pop_Y; j++)
			{
				// DBG("PITCH:", pitch , ">=", (j * tick_Y), "&&", pitch, "<", ((j+1) * tick_Y));
				if ((gyroDriver->getPitch() >= (j * tick_Y + Mmin_Y)) && (gyroDriver->getPitch() < ((j + 1) * tick_Y + Mmin_Y)))
				{
					return (j * pop_X + i); // Or smth
				}
			}
		}
	}
	return -1;
}

int PX_BLACKMAGIC::getVirtualTarget() const
{
	return virtualTarget;
}

// Write to disk
void PX_BLACKMAGIC::savePreset()
{
	// Begin: True -> Read only | False -> Read/Write
	preferences.begin("gyroStates", false);
	preferences.putBool("activeX", gyroDriver->getPitch() != 0);
	preferences.putBool("activeY", gyroDriver->getRoll() != 0);
	preferences.putBool("activeZ", gyroDriver->getYaw() != 0);
	preferences.end();
}

// Read from disk
void PX_BLACKMAGIC::loadPreset()
{
	preferences.begin("gyroStates", false);
	midiDriver->setPitchTransmission(preferences.getBool("activeX"));
	midiDriver->setRollTransmission(preferences.getBool("activeY"));
	midiDriver->setYawTransmission(preferences.getBool("activeZ"));
	preferences.end();
}