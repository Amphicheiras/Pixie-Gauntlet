#include <PX_HARDWARE/PX_GYRO.h>
#include <UF0_DBG.h>

PX_GYRO::PX_GYRO() {}

void PX_GYRO::begin()
{
	DBG("Searching for BNO..");
	// Assigning the structure to hold information about the device
	BNO_Init(&myBNO);
	DBG(".. found it");
	// Configuration to NDoF mode
	bno055_set_operation_mode(OPERATION_MODE_NDOF);
	DBG("Place towards your front & gimme sec...");
	// A moment for gyro to wake up
	delay(200);
	DBG("Good!");
	// Update Euler data into the structure
	bno055_read_euler_hrp(&myEulerData);
	// Get heading deviation
	resetHeading();
	DBG("Init Heading:", initHeading);
	// To see heading value
}

void PX_GYRO::loop()
{
	// Acquire IMU data
	getSensorReadings();
	// Calculate spatial data
	getMotionTracking();

	// Leave for drum hit optimization
	if (accelerationZ > highestAccelZ)
		highestAccelZ = accelerationZ;
	if (accelerationZ - previousAccelZ > highestDiffZ)
		highestDiffZ = accelerationZ - previousAccelZ;
	if ((millis() - GYRO_t0) > 1)
	{
		// DBG("");
		// DBG("Current acceleration Z:", accelerationZ);
		// DBG("Previous acceleration Z:", previousAccelZ);
		DBG("Current - Previous acceleration Z:", accelerationZ - previousAccelZ);
		// DBG("Highest acceleration Z:", highestAccelZ);
		// DBG("Highest diff in acceleration Z:", highestDiffZ);
		GYRO_t0 = millis();
	}
}

void PX_GYRO::getMotionTracking()
{
	// Elapsed time since last measurement
	unsigned long currentTime = millis();
	float deltaTime = (currentTime - lastTime2) / 1000.0; // Convert to seconds
	lastTime2 = currentTime;
	// Update velocity by integrating acceleration (v = v0 + a * dt)
	velocityX += accelerationX * deltaTime;
	velocityY += accelerationY * deltaTime;
	velocityZ += accelerationZ * deltaTime;
	// Update position by integrating velocity (p = p0 + v * dt)
	positionX += velocityX * deltaTime;
	positionY += velocityY * deltaTime;
	positionZ += velocityZ * deltaTime;
	// DBG("Position X: ", positionX, " m, Y: ", positionY, " m, Z: ", positionZ, " m");
}

void PX_GYRO::resetHeading()
{
	// Yaw is based on a shifted initHeading
	initHeading = myEulerData.h / 16.00;
}

double PX_GYRO::getRelativeHeading(double globalHeading, double headingDrift)
{
	// Shift initial heading by 'headingDrift' degrees
	double localHeading = (globalHeading - headingDrift);
	// Make cyclical
	if (localHeading >= 360)
		localHeading = localHeading - 360;
	if (localHeading < 0)
		localHeading = localHeading + 360;
	// Map [0, 360) -> [-180, 180)
	if (localHeading > 180)
		localHeading = localHeading - 360;
	return localHeading;
}

void PX_GYRO::getSensorReadings()
{
	// For drum hit etc.
	previousAccelX = accelerationX;
	previousAccelY = accelerationY;
	previousAccelZ = accelerationZ;
	// Update Euler data into the structure
	bno055_read_euler_hrp(&myEulerData);
	yaw = getRelativeHeading((myEulerData.h / 16.00), initHeading);
	pitch = myEulerData.r / 16.00;
	roll = myEulerData.p / 16.00;
	// DBG("YPR:", yaw, pitch, roll);
	// Update Accelerometer data into the structure
	bno055_read_linear_accel_xyz(&myLinearAccelData);
	accelerationX = myLinearAccelData.x / 100.0;
	accelerationY = myLinearAccelData.y / 100.0;
	accelerationZ = myLinearAccelData.z / 100.0;
	// DBG("Lin Acc:", axRaw, ayRaw, azRaw);
	// Update Quaternion data into the structure
	bno055_read_quaternion_wxyz(&myQuaternionData);
	q0 = myQuaternionData.w / 16384.0;
	q1 = myQuaternionData.x / 16384.0;
	q2 = myQuaternionData.y / 16384.0;
	q3 = myQuaternionData.z / 16384.0;
}

// Drum hit gesture
int PX_GYRO::drumHit()
{
	if ((millis() - GYRO_t1) > drumHitDebounce)
	{
		accelDiffZ = accelerationZ - previousAccelZ;
		previousAccelZ = accelerationZ;
		if (accelDiffZ > drumHitThreshold)
		{
			// If hit, return velocity!
			return map(accelDiffZ - drumHitThreshold, 0, 20, 0, 127);
		}
		else
		{
			return -1;
		}
		GYRO_t1 = millis();
	}
	return -1;
}

float PX_GYRO::getPitch()
{
	return pitch;
}

float PX_GYRO::getRoll()
{
	return roll;
}

float PX_GYRO::getYaw()
{
	return yaw;
}

float PX_GYRO::getAccelerationX()
{
	return accelerationX;
}

float PX_GYRO::getAccelerationY()
{
	return accelerationY;
}

float PX_GYRO::getAccelerationZ()
{
	return accelerationZ;
}

float PX_GYRO::getVelocityX()
{
	return velocityX;
}

float PX_GYRO::getVelocityY()
{
	return velocityY;
}

float PX_GYRO::getVelocityZ()
{
	return velocityZ;
}

float PX_GYRO::getPositionX()
{
	return positionX;
}

float PX_GYRO::getPositionY()
{
	return positionY;
}

float PX_GYRO::getPositionZ()
{
	return positionZ;
}

bool PX_GYRO::getDirectionX()
{
	return velocityX > 0;
}

bool PX_GYRO::getDirectionY()
{
	return velocityY > 0;
}

bool PX_GYRO::getDirectionZ()
{
	return velocityZ > 0;
}