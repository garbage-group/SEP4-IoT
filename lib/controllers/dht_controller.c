#include "pc_comm.h"
#include "dht11.h"
#include <stdio.h>
#include "wifi.h"
#include <string.h>
#include "dht_controller.h"

void getHumidity(uint8_t humidity_integer, uint8_t humidity_decimal)
{
        char carray[128];
        sprintf(carray, "humid:%d.%d\n", humidity_integer, humidity_decimal);
        wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
        pc_comm_send_string_blocking(carray);
}

void getTemperature(uint8_t temperature_integer, uint8_t temperature_decimal)
{
        char carray[128];
        sprintf(carray, "tempe:%d.%d\n", temperature_integer, temperature_decimal);
        wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
        pc_comm_send_string_blocking(carray);
}

void throwDHTError(DHTErrorType errorType)
{
        char carray[128];
        const char *errorMessage;

        switch (errorType)
        {
        case ERROR_READ_SENSOR:
                errorMessage = "Failed to read data from DHT11 sensor\n";
                break;
        case ERROR_INVALID_DATA:
                errorMessage = "Invalid data format from DHT11 sensor\n";
                break;
        case ERROR_BUFFER_OVERFLOW:
                errorMessage = "Buffer overflow encountered\n";
                break;
        case ERROR_INVALID_DATA_TYPE:
                errorMessage = "Invalid data type\n";
                break;
        default:
                errorMessage = "Unknown error\n";
        }

        sprintf(carray, "%s", errorMessage);
        wifi_command_TCP_transmit((uint8_t *)carray, strlen(carray));
        pc_comm_send_string_blocking(carray);
}
