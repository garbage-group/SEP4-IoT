#include <activate_buzzer.h>
#include <tone.h>
#include <stdio.h>
#include <utils.h>

void activateBuzzer()
{
    tone_play_starwars_tuned();
    char carray[250];
    sprintf(carray, "OK");
    sendViaTCP(carray);
}