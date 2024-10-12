#include <Arduino.h>
// Enterprise
#define UF0_DBG Serial
#include <UF0_DBG.h>
#include <PX_HARDWARE/PX_POWER.h>
#include <PX_HARDWARE/PX_BUTTON.h>
#include <PX_HARDWARE/PX_GYRO.h>
#include <PX_MUSIC/PX_MIDI.h>
#include <PX_OS/PX_FSM.h>
#include <PX_OS/PX_BLACKMAGIC.h>
#include <PX_NETWORK/PX_WEBSERVER.h>
#include <PX_NETWORK/PX_WIFI.h>

static PX_POWER powerDriver;
static PX_BUTTON buttonDriver;
static PX_GYRO gyroDriver;
static PX_MIDI midiDriver;
static PX_FSM fsmDriver;
static PX_BLACKMAGIC blackmagic;
static PX_WEBSERVER serverDriver;
static PX_WiFi wifiDriver;

void setup()
{
	DBG_SETUP(115200);
	delay(1000);
	DBG(F("=================== Das Booting =================="));
	Wire.begin(I2C_SDA, I2C_SCL);
	DBG(F("=================== BOOT POWER ================="));
	powerDriver.begin();
	DBG(F("=================== BOOT BUTTONS ================="));
	buttonDriver.setDrivers(&midiDriver);
	buttonDriver.begin();
	DBG(F("==================== BOOT GYRO ==================="));
	gyroDriver.begin();
	DBG(F("=================== BOOT WIFI ===================="));
	wifiDriver.setDrivers(&serverDriver);
	wifiDriver.begin();
	DBG(F("================= BOOT WEBSERVER ================="));
	serverDriver.setDrivers(&powerDriver,
							&midiDriver,
							&blackmagic,
							&gyroDriver,
							&wifiDriver);
	serverDriver.begin();
	DBG(F("==================== BOOT MIDI ==================="));
	midiDriver.setDrivers(&gyroDriver, &blackmagic);
	midiDriver.begin();
	DBG(F("==================== BOOT FSM ===================="));
	fsmDriver.setDrivers(&gyroDriver, &midiDriver);
	fsmDriver.begin();
	DBG(F("================= BOOT BLACKMAGIC ================"));
	blackmagic.setDrivers(&gyroDriver, &midiDriver);
	blackmagic.begin();
	DBG(F("================== BOOT COMPLETE ================="));
}

void loop()
{
	buttonDriver.loop();
	gyroDriver.loop();
	serverDriver.loop();
	midiDriver.loop();
	fsmDriver.loop();
	blackmagic.loop();
}