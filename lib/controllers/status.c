/**
 * @file  status.c
 * @brief Controller for getting the status of the ATmega2560
 *
 * This file provides the implementation of sending the status of the Arduino
 */
#include "status.h"
#include "dht11.h"
#include <string.h>
#include "hc_sr04.h"
#include "wifi.h"
#include "pc_comm.h"
#include "utils.h"
/**
 * @brief Sends an OK Message to the Cloud via TCP connection
 * 
 */
void sendOk()
{
    char carray[128];
    sprintf(carray, "statu:OK");
    wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
    pc_comm_send_string_blocking(carray);
}

/**
 * @brief Send an NOT OK Message to the Cloud via TCP connection
 * 
 */
void sendNotOk()
{
    char carray[128];
    sprintf(carray, "statu:NOT OK");
    wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
    pc_comm_send_string_blocking(carray);
}