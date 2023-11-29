#include "status.h"
#include "dht11.h"
#include <string.h>
#include "hc_sr04.h"
#include "wifi.h"
#include "pc_comm.h"
#include <avr/delay.h>

void sendOk(){
    char carray[128];
    sprintf(carray, "statu:OK");
    wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
    pc_comm_send_string_blocking(carray);
}

void sendNotOk(){
    char carray[128];
    sprintf(carray, "statu:NOT OK");
    wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
    pc_comm_send_string_blocking(carray);
}