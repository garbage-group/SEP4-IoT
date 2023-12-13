#include "unity.h"

#include "status.h"
#include <stdio.h>
#include <stdint.h>
#include "dht11.h"
#include <string.h>
#include "hc_sr04.h"
#include "wifi.h"
#include "pc_comm.h"
#include "../fff.h"

// Create mocks
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(WIFI_ERROR_MESSAGE_t, wifi_command_TCP_transmit, uint8_t *, uint16_t);
FAKE_VOID_FUNC(pc_comm_send_string_blocking, char *);

void setUp()
{
    // Resetting the FFF globals and fake functions
    RESET_FAKE(wifi_command_TCP_transmit);
    RESET_FAKE(pc_comm_send_string_blocking);
    FFF_RESET_HISTORY();
}

void tearDown()
{
}

void test_sendOk()
{
    sendOk();

    // Check if wifi_command_TCP_transmit and pc_comm_send_string_blocking were called once each
    TEST_ASSERT_EQUAL(1, wifi_command_TCP_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(1, pc_comm_send_string_blocking_fake.call_count);

    // Check if the correct message was sent
    TEST_ASSERT_EQUAL_STRING("statu:OK", wifi_command_TCP_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL_STRING("statu:OK", pc_comm_send_string_blocking_fake.arg0_val);
}

void test_sendNotOk()
{
    sendNotOk();

    // Check if wifi_command_TCP_transmit and pc_comm_send_string_blocking were called once each
    TEST_ASSERT_EQUAL(1, wifi_command_TCP_transmit_fake.call_count);
    TEST_ASSERT_EQUAL(1, pc_comm_send_string_blocking_fake.call_count);

    // Check if the correct message was sent
    TEST_ASSERT_EQUAL_STRING("statu:NOT OK", wifi_command_TCP_transmit_fake.arg0_val);
    TEST_ASSERT_EQUAL_STRING("statu:NOT OK", pc_comm_send_string_blocking_fake.arg0_val);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_sendOk);
    RUN_TEST(test_sendNotOk);
    return UNITY_END();
}
