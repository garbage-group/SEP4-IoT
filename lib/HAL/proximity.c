#include "pc_comm.h"
#include <avr/delay.h>
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
#include "proximity.h"
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>

uint16_t dtb;          // ditance to bottom
double fillth = 100.0; // fill threshold when bin is considered full
uint16_t aht = 0;      // the absolute height of the trash
double relative = 0;
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
    // cli();
    dtb = hc_sr04_takeMeasurement();
    // sei();
    sprintf(carray, "%d\n", dtb);
    sendViaTCP(carray);
    sprintf(carray, "Distance to bottom of bin is: %d mm\n", dtb);
    pc_comm_send_string_blocking(carray);
}

void getCurrentLevel()
{ // Calculating fill level of bin
    char carray[128];
    uint16_t level = hc_sr04_takeMeasurement();
    dtostrf(level, 3, 1, carray);
    pc_comm_send_string_blocking(carray);
    aht = dtb - level;
    dtostrf(aht, 3, 1, carray);
    pc_comm_send_string_blocking(carray);
    currentLevel = (aht / dtb) * 100; // relative to full bin
    dtostrf(currentLevel, 3, 1, carray);
    pc_comm_send_string_blocking(carray);

    relative = (currentLevel / fillth) * 100;

    dtostrf(relative, 3, 1, carray);
    pc_comm_send_string_blocking(carray);
    sendViaTCP(carray);
}

void setThreshold(double threshold)
{
    char carray[128];
    // sets global variable to given threshold
    fillth = threshold; // because threhsold format is precentage double ex: 70.0. This is explicitly defined in the interfacing contract :)

    dtostrf(threshold, 3, 3, carray);
    pc_comm_send_string_blocking(carray);
    sendViaTCP("setft:OK\n");
}