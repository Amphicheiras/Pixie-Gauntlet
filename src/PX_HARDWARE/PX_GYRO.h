/*
	*** GYRO related operations ***

		  PX_GYRO class:

		  resetHeading:
			Update initHeading

		  getRelativeHeading:
			Get controller's "North" heading based on initHeading

		  drumHit:
			Calculate if drum is hit
*/

#pragma once

#include <BNO055_support.h>

#define WIRE_PORT Wire

class PX_GYRO
{
private:
	// Struct for BNO055 info
	struct bno055_t myBNO;
	// Struct for Euler data
	struct bno055_euler myEulerData;
	// Struct for Quaternia data
	struct bno055_quaternion myQuaternionData;
	// Struct for Accel data
	struct bno055_accel myAccelData;
	// Struct for Linear Accel data
	struct bno055_linear_accel myLinearAccelData;

	// Constants
	const double GRAVITY = 9.81;	   // Acceleration due to gravity (m/s^2)
	const double GRAVITY_LSB = 1000.0; // Conversion factor for raw data to g
	// Acceleration calibration offsets
	double accelX_offset = 0.0;
	double accelY_offset = 0.0;
	double accelZ_offset = 0.0;
	// Gravity calibration offsets
	double gravityX_offset = 0.0;
	double gravityY_offset = 0.0;
	double gravityZ_offset = 0.0;
	double axFilter = 0.0;
	double ayFilter = 0.0;
	double azFilter = 0.0;
	double prevOutputX = 0.0;
	double prevOutputY = 0.0;
	double prevOutputZ = 0.0;
	// Calibration parameters
	const int CALIBRATION_SAMPLES = 100;
	// YPR raw values
	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;
	// Quaternia values
	double q0 = 0.0;
	double q1 = 0.0;
	double q2 = 0.0;
	double q3 = 0.0;
	// XYZ raw acceleration values
	double axRaw = 0.0;
	double ayRaw = 0.0;
	double azRaw = 0.0;
	// XYZ calibrated acceleration values
	double axCal = 0.0;
	double ayCal = 0.0;
	double azCal = 0.0;
	// XYZ g compensated acceleration values
	double axComp = 0.0;
	double ayComp = 0.0;
	double azComp = 0.0;
	// XYZ position raw values
	double xPosition = 0.0;
	double yPosition = 0.0;
	double zPosition = 0.0;
	// Heading deviation from Svalbard
	double initHeading = 0.0;
	// Acceleration MATH for drum hit
	double oldAccelX{0}, newAccelX{0}, highestAccelX{0}, highestDiffX{0}, accelDiffX{0};
	double oldAccelY{0}, newAccelY{0}, highestAccelY{0}, highestDiffY{0}, accelDiffY{0};
	double oldAccelZ{0}, newAccelZ{0}, highestAccelZ{0}, highestDiffZ{0}, accelDiffZ{0};
	// For drum hit
	int hit{-1};
	int drumHitDebounce{100} /*(ms)*/;
	int drumHitThreshold{650};
	// Timers
	unsigned long GYRO_t0 = millis(), GYRO_t1 = millis(), GYRO_t_accel = millis(), lastTime = 0;
	// 3D position
	double dT = 0.0;
	double xVelocity = 0.0;
	double yVelocity = 0.0;
	double zVelocity = 0.0;
	double displacement[3] = {0.0, 0.0, 0.0};
	float orientation = 0.0;
	float positionCurrent[3] = {0.0, 0.0, 0.0};
	float positionPrevious[3] = {0.0, 0.0, 0.0};
	int accelReadTime = 20;
	// Variables to hold position and velocity
	float positionX = 0.0, positionY = 0.0, positionZ = 0.0;
	float velocityX = 0.0, velocityY = 0.0, velocityZ = 0.0;
	// Timekeeping for integration
	unsigned long lastTime2 = 0;

	void resetHeading();
	double getRelativeHeading(double globalHeading, double headingDrift);
	void getSensorReadings();
	void calibrateGravityVector();
	void calibrateAccelerometer();
	void updateRelevantPosition(float *positionCurrent, float *positionPrevious, float *displacement);
	float get3DPosition(float *positionCurrent, float *positionPrevious, float *displacement);
	void getFilteredAccel();
	// Function to compensate for gravity in all axes
	void getCompensatedGravityAccel();
	void getCalibratedSensorReadings();
	bool edgeDetection(double newAccel, double oldAccel);

public:
	PX_GYRO();

	void begin();
	void loop();
	// Drum hit gesture
	int drumHit();
	float getPitch();
	float getRoll();
	float getYaw();
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	bool getDirectionX();
	// ax, ay, az: accelerometer readings in m/s², dT: time difference between readings (ms), velocity: variable, displacement: variable
	void getDisplacement();
};