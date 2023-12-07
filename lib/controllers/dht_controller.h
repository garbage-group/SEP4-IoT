#include <stdio.h>

typedef enum
{
    ERROR_READ_SENSOR,
    ERROR_INVALID_DATA,
    ERROR_BUFFER_OVERFLOW
} DHTErrorType;

void getHumidity(uint8_t humidity_integer, uint8_t humidity_decimal);

void getTemperature(uint8_t temperature_integer, uint8_t temperature_decimal);

void throwDHTError(DHTErrorType errorType);
