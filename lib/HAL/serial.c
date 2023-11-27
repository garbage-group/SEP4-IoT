#define SERIAL_NUMBER 1234

void send_serial_TCP()
{
    char buffer[128];
    sprintf(buffer, "%d", SERIAL_NUMBER);
    wifi_command_TCP_transmit(buffer, 4);
}