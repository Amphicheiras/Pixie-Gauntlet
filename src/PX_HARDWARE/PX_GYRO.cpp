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
	delay(1000);
	DBG("Good!");
	// Update Euler data into the structure
	bno055_read_euler_hrp(&myEulerData);
	// Get heading deviation
	resetHeading();
	DBG("Init Heading:", initHeading);
	// To see heading value
	delay(1000);
	calibrateAccelerometer();
}

void PX_GYRO::loop()
{
	oldAccelX = newAccelX;
	oldAccelY = newAccelY;
	oldAccelZ = newAccelZ;

	oldVelocityX = newVelocityX;
	oldVelocityY = newVelocityY;
	oldVelocityZ = newVelocityZ;
	getSensorReadings();
	// // getCalibratedSensorReadings();
	// // getCompensatedGravityAccel();
	// // getFilteredAccel();
	// getDisplacement();

	// Read linear acceleration data
	bno055_read_linear_accel_xyz(&myLinearAccelData);

	// Convert to m/s² (BNO055 outputs in mg, so divide by 100)
	float accelX = myLinearAccelData.x / 100.0;
	float accelY = myLinearAccelData.y / 100.0;
	float accelZ = myLinearAccelData.z / 100.0;

	newAccelX = accelX;
	newAccelY = accelY;
	newAccelZ = accelZ;

	// Get the time elapsed since the last measurement
	unsigned long currentTime = millis();
	float deltaTime = (currentTime - lastTime2) / 1000.0; // Convert to seconds
	lastTime2 = currentTime;

	// Update velocity by integrating acceleration (v = v0 + a * dt)
	velocityX += accelX * deltaTime;
	velocityY += accelY * deltaTime;
	velocityZ += accelZ * deltaTime;

	newVelocityX = velocityX;
	newVelocityY = velocityY;
	newVelocityZ = velocityZ;

	// Update position by integrating velocity (p = p0 + v * dt)
	positionX += velocityX * deltaTime;
	positionY += velocityY * deltaTime;
	positionZ += velocityZ * deltaTime;

	// // Print the current position
	// Serial.print("Position X: ");
	// Serial.print(positionX);
	// Serial.print(" m, Y: ");
	// Serial.print(positionY);
	// Serial.print(" m, Z: ");
	// Serial.print(positionZ);
	// Serial.println(" m");
}

void PX_GYRO::resetHeading()
{
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
	// Update Euler data into the structure
	bno055_read_euler_hrp(&myEulerData);
	yaw = getRelativeHeading((myEulerData.h / 16.00), initHeading);
	pitch = myEulerData.r / 16.00;
	roll = myEulerData.p / 16.00;
	// DBG("YPR:", yaw, pitch, roll);
	// Update Accelerometer data into the structure
	bno055_read_linear_accel_xyz(&myLinearAccelData);
	axRaw = myLinearAccelData.x / 100.0;
	ayRaw = myLinearAccelData.y / 100.0;
	azRaw = myLinearAccelData.z / 100.0;
	// DBG("Lin Acc:", axRaw, ayRaw, azRaw);
	// Update Quaternion data into the structure
	bno055_read_quaternion_wxyz(&myQuaternionData);
	q0 = myQuaternionData.w / 16384.0;
	q1 = myQuaternionData.x / 16384.0;
	q2 = myQuaternionData.y / 16384.0;
	q3 = myQuaternionData.z / 16384.0;
}

void PX_GYRO::calibrateGravityVector()
{
	Serial.println("Calibrating accelerometer...");

	double gravityX_sum = 0.0;
	double gravityY_sum = 0.0;
	double gravityZ_sum = 0.0;

	// Calculate bias by comparing readings to gravity vector
	for (int i = 0; i < CALIBRATION_SAMPLES; i++)
	{
		// Calculate gravity vector components based on quaternion
		getSensorReadings();
		gravityX_sum += 2 * (q1 * q3 - q0 * q2);
		gravityY_sum += 2 * (q0 * q1 + q2 * q3);
		gravityZ_sum += (q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) - 1;
		DBG(gravityX_sum, gravityY_sum, gravityZ_sum);
		delay(10);
	}

	// Average the biases
	gravityX_offset = gravityX_sum / CALIBRATION_SAMPLES;
	gravityY_offset = gravityY_sum / CALIBRATION_SAMPLES;
	gravityZ_offset = gravityZ_sum / CALIBRATION_SAMPLES;

	DBG("Calibration completed. Offsets: X: ", gravityX_offset, ", Y: ", gravityY_offset, ", Z: ", gravityZ_offset);
}

void PX_GYRO::calibrateAccelerometer()
{
	Serial.println("Calibrating accelerometer...");

	double accelX_sum = 0.0;
	double accelY_sum = 0.0;
	double accelZ_sum = 0.0;

	// Must be while because ICM data is not always ready
	int i = 0;
	while (i < CALIBRATION_SAMPLES)
	{
		bno055_read_linear_accel_xyz(&myLinearAccelData);
		accelX_sum += myLinearAccelData.x;
		accelY_sum += myLinearAccelData.y;
		accelZ_sum += myLinearAccelData.z;
		i++;
		delay(10); // Short delay between samples
	}

	// Assume that when flat, X and Y should be 0g and Z should be approximately 1g
	accelX_offset = accelX_sum / CALIBRATION_SAMPLES;
	accelY_offset = accelY_sum / CALIBRATION_SAMPLES;
	accelZ_offset = (accelZ_sum / CALIBRATION_SAMPLES) - 1.0;

	Serial.println("Calibration completed. Offsets: X: ");
	Serial.print(accelX_offset);
	Serial.print(" g, Y: ");
	Serial.print(accelY_offset);
	Serial.print(" g, Z: ");
	Serial.print(accelZ_offset);
	Serial.println(" g");
}

// ax, ay, az: accelerometer readings in m/s², dT: time difference between readings (ms), velocity: variable, displacement: variable
void PX_GYRO::getDisplacement()
{
	// Time step (in seconds)
	unsigned long currentTime = millis();
	dT = (currentTime - lastTime) / 1000.0;
	lastTime = currentTime;
	// Update values if quaternia are ready
	if (!std::isnan(q0))
	{
		// // Update velocity by integrating acceleration (m/s)
		// xVelocity += axFilter * dT;
		// yVelocity += ayFilter * dT;
		// zVelocity += azFilter * dT;
		// Update velocity by integrating acceleration (m/s)
		xVelocity += axRaw * dT;
		yVelocity += ayRaw * dT;
		zVelocity += azRaw * dT;
		// Update displacement by integrating velocity (m)
		displacement[0] += xVelocity * dT;
		displacement[1] += yVelocity * dT;
		displacement[2] += zVelocity * dT;
	}

	DBG("ax:", axComp, "ay:", ayComp, "az:", azComp, "velocity X:", xVelocity, ", velocity Y:", yVelocity, ", velocity Z:", zVelocity, ", displacement X:", displacement[0], ", displacement Y:", displacement[1], ", displacement Z:", displacement[2]);
}

void PX_GYRO::updateRelevantPosition(float *positionCurrent, float *positionPrevious, float *displacement)
{
	getDisplacement();
	positionCurrent[0] = positionPrevious[0] + displacement[0];
	positionCurrent[1] = positionPrevious[1] + displacement[1];
	positionCurrent[2] = positionPrevious[2] + displacement[2];
}

float PX_GYRO::get3DPosition(float *positionCurrent, float *positionPrevious, float *displacement)
{
	updateRelevantPosition(positionCurrent, positionPrevious, displacement);
	return *positionCurrent;
}

void PX_GYRO::getFilteredAccel()
{
	double alpha = 0.1;

	// Update values if quaternia are ready
	if (!std::isnan(axComp) && !std::isnan(ayComp) && !std::isnan(azComp))
	{
		axFilter = alpha * axComp + (1.0 - alpha) * prevOutputX;
		ayFilter = alpha * ayComp + (1.0 - alpha) * prevOutputY;
		azFilter = alpha * azComp + (1.0 - alpha) * prevOutputZ;
		prevOutputX = axFilter;
		prevOutputY = ayFilter;
		prevOutputZ = azFilter;
	}

	// DBG("Filtered", axFilter, ayFilter, azFilter, "Comp", axComp, ayComp, azComp);
}

// Function to compensate for gravity in all axes
void PX_GYRO::getCompensatedGravityAccel()
{
	// Gravity vector components (normalized)
	double gravityX = 2 * (q1 * q3 - q0 * q2);
	double gravityY = 2 * (q0 * q1 + q2 * q3);
	double gravityZ = (q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);

	// Subtract the gravity components from accelerometer readings
	// axComp = axCal - gravityX;
	// ayComp = ayCal - gravityY;
	// azComp = azCal - gravityZ;
	axComp = axRaw - gravityX;
	ayComp = ayRaw - gravityY;
	azComp = azRaw - gravityZ;
	// DBG("Compensated", axComp, ayComp, azComp);
}

void PX_GYRO::getCalibratedSensorReadings()
{
	axCal = axRaw - accelX_offset;
	ayCal = ayRaw - accelY_offset;
	azCal = azRaw - accelZ_offset;
	// DBG(" Calibrated", axCal, ayCal, azCal);
}

bool PX_GYRO::edgeDetection(double newAccel, double oldAccel)
{
	if (newAccel * oldAccel < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Drum hit gesture
int PX_GYRO::drumHit()
{
	if ((millis() - GYRO_t1) > drumHitDebounce)
	{
		axRaw = newAccelZ;
		accelDiffZ = newAccelZ - oldAccelZ;
		oldAccelZ = newAccelZ;
		if (accelDiffZ > drumHitThreshold)
		{
			// If hit, return velocity!
			return map(accelDiffZ - drumHitThreshold, 0, 2000, 0, 127);
		}
		else
		{
			return -1;
		}
		GYRO_t1 = millis();
	}

	// // Leave for drum hit optimization
	// if (newAccelZ > highestAccelZ)
	//   highestAccelZ = newAccelZ;
	// if (newAccelZ - oldAccelZ > highestDiff)
	//   highestDiff = newAccelZ - oldAccelZ;
	// if ((millis() - GYRO_t0) > 1000){
	//     DBG("");
	//     DBG("New:", newAccelZ);
	//     DBG("Old:", oldAccelZ);
	//     DBG("New - old:", newAccelZ - oldAccelZ);
	//     DBG("High:", highestAccelZ);
	//     DBG("Highest diff:", highestDiff);
	//     GYRO_t0 = millis();
	// }
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
	return axRaw;
}

float PX_GYRO::getAccelerationY()
{
	return ayRaw;
}

float PX_GYRO::getAccelerationZ()
{
	return azRaw;
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