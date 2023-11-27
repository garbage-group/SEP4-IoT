#include "pc_comm.h"
#include <avr/delay.h>
#include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
uint16_t dob;
uint16_t fillth;

void calibrateDevice(){
    dob = hc_sr04_takeMeasurement();
   
}

void getCurrentLevel(uint16_t current_level, char* carray){
    uint16_t level = dob;

}

void setThreshold(){

}

