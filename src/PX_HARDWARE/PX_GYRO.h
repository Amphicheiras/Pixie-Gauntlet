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
	double accelerationX = 0.0;
	double accelerationY = 0.0;
	double accelerationZ = 0.0;
	// Velocity
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	float velocityZ = 0.0f;
	// Position
	float positionX = 0.0f;
	float positionY = 0.0f;
	float positionZ = 0.0f;
	// Timekeeping for integration
	unsigned long lastTime2 = 0;
	// Heading deviation from Svalbard
	double initHeading = 0.0;
	// Acceleration MATH for drum hit
	int hit = -1;
	int drumHitDebounce = 100; /*(ms)*/
	int drumHitThreshold = 10;
	// X
	double previousAccelX = 0.0;
	double highestAccelX = 0.0;
	double highestDiffX = 0.0;
	double accelDiffX = 0.0;
	// Y
	double previousAccelY = 0.0;
	double highestAccelY = 0.0;
	double highestDiffY = 0.0;
	double accelDiffY = 0.0;
	// Z
	double previousAccelZ = 0.0;
	double highestAccelZ = 0.0;
	double highestDiffZ = 0.0;
	double accelDiffZ = 0.0;
	// Timers
	unsigned long GYRO_t0 = millis(), GYRO_t1 = millis(), GYRO_t_accel = millis(), lastTime = 0;

	void resetHeading();
	double getRelativeHeading(double globalHeading, double headingDrift);
	void getSensorReadings();
	void getMotionTracking();

public:
	PX_GYRO();

	void begin();
	void loop();
	// Drum hit gesture
	int drumHit();
	float getPitch();
	float getRoll();
	float getYaw();
	float getAccelerationX();
	float getAccelerationY();
	float getAccelerationZ();
	float getVelocityX();
	float getVelocityY();
	float getVelocityZ();
	float getPositionX();
	float getPositionY();
	float getPositionZ();
	bool getDirectionX();
	bool getDirectionY();
	bool getDirectionZ();
};