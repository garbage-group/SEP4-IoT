#include "pc_comm.h"
#include <avr/delay.h>
#include "dht11.h"
#include <stdio.h>
#include "wifi.h"

int main(){
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    char carray[128];
    pc_comm_init(9600, NULL);
    dht11_init();
    wifi_init();
    wifi_command_join_AP("LauritsesHotSpot","23571113");  
    wifi_command_create_TCP_connection("192.168.127.202",23,NULL,NULL); 

    while (1)
    {
        
        pc_comm_send_string_blocking("Hello from the arduino\n");
        _delay_ms(1000);
        DHT11_ERROR_MESSAGE_t result = dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);
        

    if (result == DHT11_OK) {
        sprintf(carray, "it's currently %d,%d degrees celcius\n",temperature_integer, temperature_decimal);
        wifi_command_TCP_transmit(carray, 37);
        //pc_comm_send_string_blocking(carray);
        _delay_ms(2000);
    } else {
        sprintf(carray, "Failed to read data from DHT11 sensor\n");
    }
    }
    
}