#include <activate_buzzer.h>
#include <tone.h>
#include <stdio.h>
#include <utils.h>

/**
 * @brief Method which calls upon tone_play_starwars_tuned() functionality in the tone.h driver header
 * once tone_play_starwars_tuned() is called,  the method sends "OK"
 *
 */
void activateBuzzer()
{
    tone_play_starwars_tuned();
    char carray[250];
    sprintf(carray, "OK");
    sendViaTCP(carray);
}