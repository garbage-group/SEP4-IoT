#include "unity.h"

#include "status.h"
#include <stdio.h>
#include <stdint.h>
#include "dht11.h"
#include <string.h>
#include "hc_sr04.h"
#include "wifi.h"
#include "pc_comm.h"
#include "serial.h"
#include "../fff.h"

// Create mocks
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_TCP_transmit, uint8_t *, uint16_t);

void setUp()
{
    // Resetting the FFF globals and fake functions
    RESET_FAKE(wifi_command_TCP_transmit);
    FFF_RESET_HISTORY();
}

void tearDown()
{
}

void test_send_serial_TCP(void)
{

    // Act: Call the function under test
    send_serial_TCP();

    // Assert: Check if wifi_command_TCP_transmit was called correctly
    TEST_ASSERT_EQUAL(1, wifi_command_TCP_transmit_fake.call_count);

    char expected_buffer[] = "1234\n";
    uint8_t *actual_buffer = wifi_command_TCP_transmit_fake.arg0_val;
    uint16_t actual_length = wifi_command_TCP_transmit_fake.arg1_val;
    // Check that the length is correct
    TEST_ASSERT_EQUAL(strlen(expected_buffer), actual_length);

    // Check each byte of the buffer
    for (size_t i = 0; i < actual_length; ++i)
    {
        TEST_ASSERT_EQUAL_MESSAGE(expected_buffer[i], actual_buffer[i], "Buffer content mismatch");
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_send_serial_TCP);
    UNITY_END();
}