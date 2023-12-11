#include "unity.h"

#include "../fff.h"

#include <stdio.h>
#include <stdint.h>
#include "hc_sr04.h"
#include "proximity.h"

// Globals to hold stub data
char last_wifi_command[128];
int last_wifi_length;
char last_pc_command[128];
typedef enum
{
    PROXIMITY,
    CALIBRATE,
    THRESHOLD
} DataType;

char last_wifi_output[128];
char last_pc_output[128];

// Stub functions to isolate tests from external dependencies
void wifi_command_TCP_transmit(const char *command, int length)
{
    // null-termination
    strncpy(last_wifi_command, command, sizeof(last_wifi_command));
    last_wifi_length = length;
    // Clear the previous output
    memset(last_wifi_output, 0, sizeof(last_wifi_output));
    // output=string and & null-terminated
    strncpy(last_wifi_output, (const char *)command, length);
    last_wifi_output[length] = '\0';
}

void pc_comm_send_string_blocking(const char *command)
{
    strncpy(last_pc_command, command, sizeof(last_pc_command));
    // Clear the previous output
    memset(last_pc_output, 0, sizeof(last_pc_output));
    // Copy the output to the global variable
    strncpy(last_pc_output, command, sizeof(last_pc_output) - 1);
}

// Bitwise operation to extract the measurement parts
void processData(uint16_t rawData, char *buffer, size_t bufferSize, DataType type)
{
    // Extracting the integer part (higher 8 bits)
    uint8_t measured_integer = (rawData >> 8) & 0xFF;

    // Extracting the decimal part (lower 8 bits)
    uint8_t measured_decimal = rawData & 0xFF;

    if (type == PROXIMITY)
    {
        snprintf(buffer, bufferSize, "level:%u.%u\n", measured_integer, measured_decimal);
    }
    wifi_command_TCP_transmit(buffer, strlen(buffer));
    pc_comm_send_string_blocking(buffer);
}

void processCharData(char *rawData, char *buffer, size_t bufferSize, DataType type)
{
    if (rawData != NULL && strcmp(rawData, "OK") == 0) {
        if (type == CALIBRATE) {
            snprintf(buffer, bufferSize, "calib:OK\n");
        } else if (type == THRESHOLD) {
            snprintf(buffer, bufferSize, "setft:OK\n");
        }
    } else{
        if(type == CALIBRATE){
            snprintf(buffer, bufferSize, "calib:FAILED\n");
        }
        if(type == THRESHOLD){
            snprintf(buffer, bufferSize, "setft:FAILED\n");
        }
    }

    wifi_command_TCP_transmit(buffer, strlen(buffer));
    pc_comm_send_string_blocking(buffer);
}

void setUp()
{
    // Reset the fake function before each test
    memset(last_wifi_command, 0, sizeof(last_wifi_command));
    last_wifi_length = 0;
    memset(last_pc_command, 0, sizeof(last_pc_command));
    memset(last_wifi_output, 0, sizeof(last_wifi_output));
    memset(last_pc_output, 0, sizeof(last_pc_output));
}

void tearDown(void)
{
}

//-------------------------------------TEST_CASES-------------------------------------------//

//-----------------UNIT_TEST_TO_MOCK_HUMIDITY_DATA_USING_BIT_OPERATORS------------------//

void test_processProximityData(void)
{
    char buffer[128];
    //  rawData is a 16-bit number
    // Higher 8 bits are integer part, lower 8 bits are decimal part
    uint16_t rawData = 0x2D00; // is 45.0

    processData(rawData, buffer, sizeof(buffer), PROXIMITY);
    TEST_ASSERT_EQUAL_STRING("level:45.0\n", buffer);
    TEST_ASSERT_EQUAL_STRING("level:45.0\n", last_wifi_command);
    TEST_ASSERT_EQUAL_INT(strlen("level:45.0\n"), last_wifi_length);
    TEST_ASSERT_EQUAL_STRING("level:45.0\n", last_pc_command);
}

void test_processProximityBoundaryData(void) {
    char buffer[128];
    uint16_t rawData = 0xFFFF; // maximum value, representing 255.255

    processData(rawData, buffer, sizeof(buffer), PROXIMITY);
    TEST_ASSERT_EQUAL_STRING("level:255.255\n", buffer);
    TEST_ASSERT_EQUAL_STRING("level:255.255\n", last_wifi_command);
    TEST_ASSERT_EQUAL_STRING("level:255.255\n", last_pc_command);
}

void test_processCalibrationEmptyData(void) {
    char buffer[128];
    char rData[] = "";

    processCharData(rData, buffer, sizeof(buffer), CALIBRATE);
    TEST_ASSERT_EQUAL_STRING("calib:FAILED\n", buffer);
    TEST_ASSERT_EQUAL_STRING("calib:FAILED\n", last_wifi_command);
    TEST_ASSERT_EQUAL_STRING("calib:FAILED\n", last_pc_command);
}

void test_processCalibrationOKData(void)
{
    char buffer[128];
    char rData[] = "OK";

    processCharData(rData, buffer, sizeof(buffer), CALIBRATE);
    TEST_ASSERT_EQUAL_STRING("calib:OK\n", buffer);
    TEST_ASSERT_EQUAL_STRING("calib:OK\n", last_wifi_command);
    TEST_ASSERT_EQUAL_STRING("calib:OK\n", last_pc_command);
}

void test_processThresholdOKData(void)
{
    char buffer[128];
    char rData[] = "OK";

    processCharData(rData, buffer, sizeof(buffer), THRESHOLD);
    TEST_ASSERT_EQUAL_STRING("setft:OK\n", buffer);
    TEST_ASSERT_EQUAL_STRING("setft:OK\n", last_wifi_command);
    TEST_ASSERT_EQUAL_STRING("setft:OK\n", last_pc_command);
}

void test_processCalibrationFailedData(void)
{
    char buffer[128];
    char rData[] = "FAILED";

    processCharData(rData, buffer, sizeof(buffer), CALIBRATE);
    TEST_ASSERT_EQUAL_STRING("calib:FAILED\n", buffer);
    TEST_ASSERT_EQUAL_STRING("calib:FAILED\n", last_wifi_command);
    TEST_ASSERT_EQUAL_STRING("calib:FAILED\n", last_pc_command);
}

void test_processThresholdFailedData(void)
{
    char buffer[128];
    char rData[] = "FAILED";

    processCharData(rData, buffer, sizeof(buffer), THRESHOLD);
    TEST_ASSERT_EQUAL_STRING("setft:FAILED\n", buffer);
    TEST_ASSERT_EQUAL_STRING("setft:FAILED\n", last_wifi_command);
    TEST_ASSERT_EQUAL_STRING("setft:FAILED\n", last_pc_command);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_processProximityData);
    RUN_TEST(test_processProximityBoundaryData);
    RUN_TEST(test_processThresholdOKData);
    RUN_TEST(test_processThresholdFailedData);
    RUN_TEST(test_processCalibrationOKData);
    RUN_TEST(test_processCalibrationFailedData);
    RUN_TEST(test_processCalibrationEmptyData);

    return UNITY_END();
}