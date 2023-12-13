/**
 * @file  activate_buzzer.c
 * @brief Controller for the tone.h header file for the driver implementation for ATmega2560
 *
 * This file provides the implementation for calling upon the tone_play_starwars_tuned() int tone.h
 *
 */
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