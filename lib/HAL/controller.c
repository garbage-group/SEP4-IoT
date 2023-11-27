#include "pc_comm.h"
#include <avr/delay.h>
#include "dht11.h"
#include <stdio.h>
#include "wifi.h"

void getHuimidty(uint8_t humidity_integer, uint8_t humidity_decimal, char* carray){
        sprintf(carray, "humid:%d.%d:%s:%s:\n",humidity_integer,humidity_decimal);
        wifi_command_TCP_transmit(carray, 50);
        pc_comm_send_string_blocking(carray);
}

void getTemperature(uint8_t temperature_integer, uint8_t temperature_decimal, char *carray){
        sprintf(carray, "tempe:%d.%d:%s:\n",temperature_integer,temperature_decimal);
        wifi_command_TCP_transmit(carray, 50);
        pc_comm_send_string_blocking(carray);
}