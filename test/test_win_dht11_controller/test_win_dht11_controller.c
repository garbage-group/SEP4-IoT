#include "unity.h"

#include "../fff.h"

#include <stdio.h>
#include <stdint.h>
#include "dht11.h"
#include "dht_controller.h"

DEFINE_FFF_GLOBALS;

// Globals to hold stub data
char last_wifi_command[128];
int last_wifi_length;
char last_pc_command[128];
typedef enum
{
    TEMPERATURE,
    HUMIDITY
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

    if (rawData > 0x7FFF)
    { // 0x7FFF is the maximum valid value
        throwDHTError(ERROR_INVALID_DATA);
        return;
    }
    // Check for buffer overflow potential
    const int requiredSize = snprintf(NULL, 0, "%s:%u.%u\n", (type == TEMPERATURE) ? "tempe" : "humid", 255, 255);
    if (bufferSize < requiredSize)
    {
        throwDHTError(ERROR_BUFFER_OVERFLOW);
        return;
    }

    // Extracting the integer part (higher 8 bits)
    uint8_t measured_integer = (rawData >> 8) & 0xFF;

    // Extracting the decimal part (lower 8 bits)
    uint8_t measured_decimal = rawData & 0xFF;

    if (type == TEMPERATURE)
    {
        snprintf(buffer, bufferSize, "tempe:%u.%u\n", measured_integer, measured_decimal);
    }
    else if (type == HUMIDITY)
    {
        snprintf(buffer, bufferSize, "humid:%u.%u\n", measured_integer, measured_decimal);
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

void test_processHumidityData(void)
{
    char buffer[128];
    //  rawData is a 16-bit number
    // Higher 8 bits are integer part, lower 8 bits are decimal part
    uint16_t rawData = 0x2D09; // is 45.9

    processData(rawData, buffer, sizeof(buffer), HUMIDITY);
    TEST_ASSERT_EQUAL_STRING("humid:45.9\n", buffer);
    TEST_ASSERT_EQUAL_STRING("humid:45.9\n", last_wifi_command);
    TEST_ASSERT_EQUAL_INT(strlen("humid:45.9\n"), last_wifi_length);
    TEST_ASSERT_EQUAL_STRING("humid:45.9\n", last_pc_command);
}

//-----------------UNIT_TEST_TO_MOCK_TEMPERATURE_DATA_USING_BIT_OPERATORS------------------//

void test_processTemperatureData(void)
{
    char buffer[128];
    //  rawData is a 16-bit number
    // Higher 8 bits are integer part, lower 8 bits are decimal part
    uint16_t rawData = 0x1609; // is 22.9

    processData(rawData, buffer, sizeof(buffer), TEMPERATURE);
    TEST_ASSERT_EQUAL_STRING("tempe:22.9\n", buffer);
    TEST_ASSERT_EQUAL_STRING("tempe:22.9\n", last_wifi_command);
    TEST_ASSERT_EQUAL_INT(strlen("tempe:22.9\n"), last_wifi_length);
    TEST_ASSERT_EQUAL_STRING("tempe:22.9\n", last_pc_command);
}

//-----------------UNIT_TEST_TO_VERIFY_BEHAVIOUR_OUTSIDE_EXPECTED_FORMAT------------------//

void test_processDataWithInvalidRawData(void)
{
    char buffer[128];
    uint16_t rawData = 0xFFFF; // invalid data

    processData(rawData, buffer, sizeof(buffer), TEMPERATURE);

    TEST_ASSERT_EQUAL_STRING("Invalid data format from DHT11 sensor\n", last_wifi_output);
}

//-----------------UNIT_TEST_TO_VERIFY_BEHAVIOUR_FOR_SMALL_BUFFER_FORMAT------------------//

void test_processDataWithSmallBufferSize(void)
{
    char buffer[10];
    uint16_t rawData = 0x2D09;

    processData(rawData, buffer, sizeof(buffer), HUMIDITY);

    // check if buffer has correctly truncated string or error message
    TEST_ASSERT_EQUAL_STRING("Buffer overflow encountered\n", last_wifi_output);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_processHumidityData);
    RUN_TEST(test_processTemperatureData);
    RUN_TEST(test_processDataWithInvalidRawData);
    RUN_TEST(test_processDataWithSmallBufferSize);

    return UNITY_END();
}