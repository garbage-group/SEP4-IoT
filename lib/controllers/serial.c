#include <string.h>
#include <wifi.h>
#include <stdio.h>
#include "utils.h"
#define SERIAL_NUMBER 1234

/**
 * @brief Method which sends over the serial number of the Arduino
 *
 */
void send_serial_TCP()
{
    char buffer[128];
    sprintf(buffer, "%d\n", SERIAL_NUMBER);
    wifi_command_TCP_transmit((uint8_t *)buffer, strlen(buffer));
}
