#include "status.h"
#include "dht11.h"
#include <string.h>
#include "hc_sr04.h"

uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
uint16_t distance_result;

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

void getStatus(){
    DHT11_ERROR_MESSAGE_t result = dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);
    distance_result = hc_sr04_takeMeasurement();

    if(result == 1 && distance_result > -1 && distance_result < 4000){
        sendOk();
    } else{
        sendNotOk();
    }
}