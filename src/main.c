#include "pc_comm.h"
#include <avr/delay.h>
#include "dht11.h"
#include <stdio.h>
#include "wifi.h"

char carray[128]; 
char rarray[128];
uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

void receiveMessage(){
    DHT11_ERROR_MESSAGE_t result = dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

    if (result == DHT11_OK && strcmp(rarray, "getHumidity") == 0) {
        //sprintf(carray, "it's currently %d,%d degrees celcius\n",temperature_integer, temperature_decimal);
        sprintf(carray, "%d.%d%%\n",humidity_integer,humidity_decimal);
        wifi_command_TCP_transmit(carray, 10);
        pc_comm_send_string_blocking(carray);
    } else {
        sprintf(carray, "Failed to read data from DHT11 sensor\n");
        wifi_command_TCP_transmit(carray, 40);
    }
}

int main(){ 
    pc_comm_init(9600, NULL);
    pc_comm_send_string_blocking("start of program\n");
    dht11_init();
    wifi_init();
    _delay_ms(4000);
    WIFI_ERROR_MESSAGE_t wifiresult = wifi_command_join_AP("Securi-ty","Sucction"); 
    if(wifiresult == WIFI_OK)
    {
        pc_comm_send_string_blocking("connected\n");
    }else
    {
        pc_comm_send_string_blocking("failed to connect\n");
    }

    WIFI_ERROR_MESSAGE_t tcpResult = wifi_command_create_TCP_connection("192.168.182.11",23,receiveMessage,rarray); 
    if (tcpResult == WIFI_OK)
    {
        pc_comm_send_string_blocking("TCP connected\n");
    }else
    {
        pc_comm_send_string_blocking("TCP FAILED\n");
    }
    

    while (1)
    {        
        //pc_comm_send_string_blocking("Hello from the arduino\n");
        //sprintf(carray,"Hello from the arduino\n");
    }
    
}