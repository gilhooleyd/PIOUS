/* led.h
 *
 * Description: Module for interfacing with Pi 2's onboard LEDs.
 *
 * TODO: Add better documentation, or rewrite to make more sense.
 */

#ifndef LED_H
#define LED_H


#include "globals.h"

/* Constants */
enum {
    /* GPIO function select registers.
     * GPFSEL{m} controls 10 GPIO pins: [m*10, m*10+9].
     * Every 3 bits corresponds to one pin. */
    GPFSEL3 = 0x3F20000C,
    GPFSEL4 = 0x3F200010,

    /* GPIO set/clear registers.
     * Write 1 to bit n of GPSET/CLR{m} to set/clear GPIO pin m*32+n. */
    GPSET1  = 0x3F200020,
    GPCLR1  = 0x3F20002C,
};

/* Used to set LEDs. */
typedef enum {
    OFF = 0,
    ON = 1,
} switch_t;

/* Used to specify which LED to set. */
typedef enum {
    RED_LED = 35,       /* Red (PWR) LED GPIO pin */
    GREEN_LED = 47,     /* Green (ACT/OK) LED GPIO pin */
} led_t;


/* Initialization methods. */
bool_t led_init(void);
bool_t led_is_initialized(void);

/* Switches the LED on/off. */
void set_led(led_t led, switch_t s);

/* Blinks a bunch. */
void led_blink(void);

#endif