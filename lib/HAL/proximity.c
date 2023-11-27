#include "pc_comm.h"
#include <avr/delay.h>
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
#include "proximity.h"
uint16_t dob;    // ditance till bottom
uint16_t fillth; // fill threshold when bin is considered full

void calibrateDevice()
{
    char carray[128];
    // sets dob to the current measurement from sensor
    //  to calibrate empty distance
    dob = hc_sr04_takeMeasurement();
    sprintf(carray, "%d\n", dob);
    wifi_command_TCP_transmit(carray, 10);
    pc_comm_send_string_blocking(carray);
}

void getCurrentLevel(uint16_t *current_level)
{ // Calculating fill level of bin
    char carray[128];

    // initializing current distance measured by the sensor
    uint16_t level = hc_sr04_takeMeasurement;
    uint16_t fillLevel = 0;
    if (level <= dob)
    {
        fillLevel = ((dob - level) * 100) / dob;
    }

    *current_level = fillLevel;

    sprintf(carray, "level:%u%%\n", fillLevel);
    wifi_command_TCP_transmit(carray, 10);
    pc_comm_send_string_blocking(carray);
}

void setThreshold(uint16_t threshold)
{
    char carray[128];
    // sets global variable to given threshold
    fillth = threshold;
    sprintf(carray, "%d\n", fillth);
    wifi_command_TCP_transmit(carray, 10);
    pc_comm_send_string_blocking(carray);
}
