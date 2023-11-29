#include "pc_comm.h"
#include <avr/delay.h>
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
#include "proximity.h"
#include <string.h>
#include <stdlib.h>

uint16_t dtb;          // ditance to bottom
double fillth = 1.0;   // fill threshold when bin is considered full
double addtoFilth = 0; // the amount of distance "not used" after setting the threshold
double currentLevel;

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

    // initializing current distance measured by the sensor
    uint16_t level = hc_sr04_takeMeasurement();
    double myfloat = (double)level;
    double myfloat2 = (double)dtb;
    currentLevel = myfloat / (myfloat2 - addtoFilth);

    sprintf(carray, "level:%f\n", currentLevel);
    sendViaTCP(carray);
    pc_comm_send_string_blocking(carray);
}

void setThreshold(double threshold)
{
    char carray[128];
    // sets global variable to given threshold
    fillth = threshold / 100; // because threhsold format is precentage double ex: 70.0. This is explicitly defined in the interfacing contract :)

    addtoFilth = dtb - (dtb * fillth); // the amount of distance "lost" with the new definition of full bin specified by the threshold

    dtostrf(threshold, 3, 1, carray);
    // sprintf(carray, "%Lf\n", threshold);
    pc_comm_send_string_blocking(carray);
    sendViaTCP("setft:OK\n");
}

void printdtb()
{
    char carray[128];
    sprintf(carray, "distance till bottom is: %d\n", dtb);
    pc_comm_send_string_blocking(carray);
}