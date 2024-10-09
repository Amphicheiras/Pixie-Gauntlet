#include <Arduino.h>
#include <unity.h>
#include <PX_HARDWARE/PX_GYRO.h>

// Create an instance of PX_GYRO for testing
PX_GYRO gyro;

// Mock sensor data for the test
bno055_euler mockEulerData;
bno055_linear_accel mockLinearAccelData;

void setUp(void)
{
    // This function is run before each test
    gyro.begin(); // Initialize the gyro
}

void tearDown(void)
{
    // This function is run after each test
}

void test_gyro_displacement(void)
{
    gyro.getDisplacement(); // Call to update displacement
    // After reset, displacement should be 0
    TEST_ASSERT_EQUAL(gyro.getPositionX(), 0);
    TEST_ASSERT_EQUAL(gyro.getPositionY(), 0);
    TEST_ASSERT_EQUAL(gyro.getPositionZ(), 0);
}

void setup()
{
    UNITY_BEGIN(); // Initialize Unit Test framework
    RUN_TEST(test_gyro_displacement);
    UNITY_END(); // Close Unit Test framework
}

void loop()
{
    // Keep the loop empty for testing
}
