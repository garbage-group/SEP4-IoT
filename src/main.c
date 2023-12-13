#include "pc_comm.h"
#include <avr/delay.h>
#include "dht11.h"
#include "hc_sr04.h"
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
#include "utils.h"

/**
 * @brief Main entry point method which contein the initiation of the used headers
 *
 * @return int
 */
int main()
{
    pc_comm_init(9600, NULL);
    pc_comm_send_string_blocking("start of program\n");
    tone_init();
    dht11_init();
    hc_sr04_init();
    wifi_init();
    _delay_ms(4000);
    connect_to_wifi();
    create_TCP_connection();

    pc_comm_send_string_blocking("Hello from the arduino\n");

    //    char buffer[500];
    //    int cnt = 1;
    while (1)
    {
        /*
        if (cnt % 3 == 0)
        {
            wifi_command_close_TCP_connection();
            create_TCP_connection();
        }

        if (cnt % 5 == 0)
        {
            uint16_t distance = hc_sr04_takeMeasurement();

            sprintf(buffer, "Current distance %d mm (Calibr: %d)\n", distance, get_calibrated_value());
            pc_comm_send_string_blocking(buffer);
        }
        _delay_ms(1000);
        cnt++;*/
        // pc_comm_send_string_blocking("Hello from the arduino\n");
        // sprintf(carray,"Hello from the ardu  ino\n");
    }
}