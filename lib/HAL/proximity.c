#include "pc_comm.h"
#include <avr/delay.h>
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
uint16_t dob;    // ditance till bottom
uint16_t fillth; // fill threshold when bin is considered full

void calibrateDevice()
{
    // sets dob to the current measurement from sensor
    //  to calibrate empty distance
    dob = hc_sr04_takeMeasurement();
}

void getCurrentLevel(uint16_t *current_level, char *carray)
{ // Calculating fill level of bin

    // initializing current distance measured by the sensor
    uint16_t level = hc_sr04_takeMeasurement;
    uint16_t fillLevel = 0;
    if (level <= dob)
    {
        fillLevel = ((dob - level) * 100) / dob;
    }

    *current_level = fillLevel;

    // to print in main
    // sprintf(carray, "%u%%", fillLevel);
}

void setThreshold(uint16_t threshold)
{
    // sets global variable to given threshold
    fillth = threshold;
}

char carray[128];
char rarray[128];
uint16_t level_integer;

void receiveMessage()
{

    if (strcmp(rarray, "getCurrentLevel") == 0)
    {
        // sprintf(carray, "it's currently %d,%d degrees celcius\n",temperature_integer, temperature_decimal);
        sprintf(carray, "level:%u%%\n", level_integer);
        wifi_command_TCP_transmit(carray, 50);
        pc_comm_send_string_blocking(carray);
    }
    else
    {
        sprintf(carray, "Failed to read data from DHT11 sensor\n");
        wifi_command_TCP_transmit(carray, 40);
    }
}
