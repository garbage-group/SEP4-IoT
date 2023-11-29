#include "pc_comm.h"
#include <avr/delay.h>
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
#include "proximity.h"
#include <string.h>
#include <stdlib.h>

uint16_t dtb;          // ditance to bottom
double fillth = 100.0; // fill threshold when bin is considered full
uint16_t aht = 0;      // the absolute height of the trash
double newFull = 0;
double currentLevel = 0;

int sendViaTCP(char *buffer)
{
    return wifi_command_TCP_transmit((uint8_t *)buffer, strlen(buffer));
}

void calibrateDevice()
{
    char carray[128];
    // sets dob to the current measurement from sensor
    //  to calibrate empty distance
    dtb = hc_sr04_takeMeasurement();
    sprintf(carray, "%d\n", dtb);
    sendViaTCP(carray);
    sprintf(carray, "Distance to bottom of bin is: %d mm\n", dtb);
    pc_comm_send_string_blocking(carray);
}

void getCurrentLevel()
{ // Calculating fill level of bin
    char carray[128];
    uint16_t level = hc_sr04_takeMeasurement();
    aht = dtb - level;
    currentLevel = (aht / fillth) * 100;

    dtostrf(currentLevel, 3, 1, carray);
    sendViaTCP(carray);
    pc_comm_send_string_blocking(carray);
}

void setThreshold(double threshold)
{
    char carray[128];
    // sets global variable to given threshold
    fillth = threshold; // because threhsold format is precentage double ex: 70.0. This is explicitly defined in the interfacing contract :)

    dtostrf(threshold, 3, 1, carray);
    // sprintf(carray, "%Lf\n", threshold);
    pc_comm_send_string_blocking(carray);
    sendViaTCP("setft:OK\n");
}