#include "pc_comm.h"
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
#include "proximity.h"
#include <string.h>
#include <stdlib.h>

uint16_t dtb= 0;       // distance to bottom
double fillth = 100.0; // fill threshold when bin is considered full
uint16_t aht = 0;      // the absolute height of the trash
double relative = 0;
double currentLevel = 0;

int sendViaTCP(char *buffer)
{
    return wifi_command_TCP_transmit((uint8_t *)buffer, strlen(buffer));
}

uint16_t get_calibrated_value() { return dtb;}

void calibrateDevice()
{
    char carray[128];
    // sets dob to the current measurement from sensor
    //  to calibrate empty distance
    // cli();
    uint16_t distance = hc_sr04_takeMeasurement();
    dtb = distance;
    // sei();
    sprintf(carray, "calib:OK\n");
    sendViaTCP(carray);
    sprintf(carray, "Distance to bottom of bin is: %d mm\n", distance);
    pc_comm_send_string_blocking(carray);
}

void getCurrentLevel()
{ // Calculating fill level of bin
    char carray[128];
    char buf[256];
    uint16_t level = hc_sr04_takeMeasurement();

    // if there is more space than what was the calibrated value
    // work with the calibration
    if (level > dtb)
        level = dtb;

    aht = dtb - level;

    sprintf(carray, "Measurement %d mm calibrated: %d mm trash amount= %d mm\n", level, dtb, aht);
    pc_comm_send_string_blocking(carray);

    currentLevel = ((double)aht / (double)dtb) * 100; // relative to full bin
    dtostrf(currentLevel, 3, 1, carray);
    sprintf(buf, "Absolute trash level %s\n", carray);
    pc_comm_send_string_blocking(buf);

    relative = (currentLevel / fillth) * 100;
    dtostrf(relative, 3, 1, carray);
    sprintf(buf, "Relative trash level %s\n", carray);
    pc_comm_send_string_blocking(buf);

    sprintf(buf, "level:%s\n", carray);
    sendViaTCP(buf);
}

void setThreshold(double threshold)
{
    char carray[128];
    char buff[256];
    // sets global variable to given threshold
    fillth = threshold; // because threhsold format is precentage double ex: 70.0. This is explicitly defined in the interfacing contract :)

    dtostrf(threshold, 3, 3, carray);
    sprintf(buff, "Setting treshold to %s\n", carray);
    pc_comm_send_string_blocking(buff);

    sprintf(carray, "setft:OK");
    sendViaTCP(carray);
}