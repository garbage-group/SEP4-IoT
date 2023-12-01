#include "pc_comm.h"
#include <avr/delay.h>
#include "dht11.h"
// #include "hc_sr04.h"
#include <stdio.h>
#include "wifi.h"
#include "dht_controller.h"
#include "proximity.h"
#include "serial.h"
#include "tone.h"
#include "activate_buzzer.h"
#include "string.h"
#include "status.h"
#include <stdlib.h>

char rarray[128];
uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
uint16_t level_integer;

double parse_double(const char *string)
{
    char carray[128];
    char string_buffer[128];
    int decimal, precision;
    char *ptr;
    int power;
    double dividor = 1;
    sprintf(string_buffer, "%s", string);
    // 70.001

    char *point_position = strchr(string_buffer, '.'); // saves the pointer in openParen to the char before the value "("

    ptr = point_position + 1;
    power = strlen(ptr);
    sscanf(ptr, "%d", &precision);

    *point_position = '\0';
    sscanf(string_buffer, "%d", &decimal);

    for (int i = 1; i <= power; i++)
        dividor *= 10;

    sprintf(carray, "DEBUG: %d,%d,%d,%s,%s\n", decimal, precision, power, ptr, string);
    pc_comm_send_string_blocking(carray);
    return (double)decimal + ((double)precision / dividor);
};

void receiveMessage()
{
    DHT11_ERROR_MESSAGE_t result = dht11_get(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);
    level_integer = hc_sr04_takeMeasurement();

    if (strcmp(rarray, "getSerialNumber") == 0)
    {
        send_serial_TCP();
    }
    if (result == DHT11_OK && strcmp(rarray, "getHumidity") == 0)
    {
        getHuimidty(humidity_integer, humidity_decimal);
    }
    if (result == DHT11_OK && strcmp(rarray, "getTemperature") == 0)
    {
        getTemperature(temperature_integer, temperature_decimal);
    }
    if (strcmp(rarray, "calibrateDevice") == 0)
    {
        calibrateDevice();
    }
    if (strncmp(rarray, "setFill", 7) == 0)

    {
        char carray[200];
        const char *openParen = strchr(rarray, '('); // saves the pointer in openParen to the char before the value "("
        char *closedParen = strchr(rarray, ')');
        if (closedParen != NULL)
            *closedParen = '\0';

        pc_comm_send_string_blocking(openParen);

        pc_comm_send_string_blocking("\n");
        if (openParen != NULL)
        {
            double value;
            value = parse_double(openParen + 1);

            setThreshold(value); // The number is sent by cloud this way
        }
    }

    if (strcmp(rarray, "getCurrentLevel") == 0)
    {
        getCurrentLevel(level_integer);
    }
    if (strcmp(rarray, "activateBuzzer") == 0)
    {
        activateBuzzer();
    }
    if (strcmp(rarray, "getStatus") == 0)
    {
        if (result == DHT11_OK && (int)level_integer > 49 && (int)level_integer < 3001)
        {
            sendOk();
        }
        else
        {
            sendNotOk();
        }
    }
}

int create_TCP_connection()
{
    WIFI_ERROR_MESSAGE_t tcpResult = wifi_command_create_TCP_connection("192.168.0.68", 5663, receiveMessage, rarray);
    if (tcpResult == WIFI_OK)
    {
        pc_comm_send_string_blocking("TCP connected\n");
    }
    else
    {
        pc_comm_send_string_blocking("TCP FAILED\n");
    }
    return tcpResult;
}

int main()
{
    pc_comm_init(9600, NULL);
    pc_comm_send_string_blocking("start of program\n");
    tone_init();
    dht11_init();
    hc_sr04_init();
    wifi_init();
    _delay_ms(4000);
    WIFI_ERROR_MESSAGE_t wifiresult = wifi_command_join_AP("FTTH_MP8523", "syemRekWeed4");
    if (wifiresult == WIFI_OK)
    {
        pc_comm_send_string_blocking("connected\n");
    }
    else
    {
        pc_comm_send_string_blocking("failed to connect\n");
    }
    create_TCP_connection();

    while (1)
    {

        // wifi_command_close_TCP_connection();
        // create_TCP_connection();
        // pc_comm_send_string_blocking("Hello from the arduino\n");
        // sprintf(carray,"Hello from the arduino\n");
    }


}