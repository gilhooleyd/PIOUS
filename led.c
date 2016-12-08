/* led.c
 *
 * Description: Module for interfacing with Pi 2's onboard LEDs.
 *
 * TODO: Add better documentation, or rewrite to make more sense.
 */
#include "led.h"


/* Whether this module is initialized */
static bool_t is_initialized;


/* Initializes the LED module.               *
 * Must be called before all other methods.  *
 * Returns whether initialization succeeded. */
bool_t led_init(void) {
    unsigned int ra;

    // GPFSEL{m} controls 10 GPIO pins: [m*10, m*10+9].
    // Every 3 bits corresponds to one pin.

    // Set (turn off) green LED (GPIO pin 35).
    // 35 / 10 -> GPFSEL3; (35 % 10) * 3 -> 15th bit
    ra=GET32(GPFSEL3);
    ra&=~(7<<15);
    ra|=1<<15;
    PUT32(GPFSEL3,ra);

    // Set (turn off) red LED (GPIO pin 47).
    // 47 / 10 -> GPFSEL4; (47 % 10) * 3 -> 21st bit
    ra=GET32(GPFSEL4);
    ra&=~(7<<21);
    ra|=1<<21;
    PUT32(GPFSEL4,ra);

    is_initialized = TRUE;
    return is_initialized;
}


/* Returns whether this module was initialized */
bool_t led_is_initialized(void) {
    return is_initialized;
}


/* Turns the LED on, then back off after a brief time. */
void set_led(led_t led_pin, switch_t s) {
    // You clear to turn on and set to turn off.
    // Just roll with it.
    if (s) {
        PUT32(GPCLR1,1<<(led_pin-32));
    } else {
        PUT32(GPSET1,1<<(led_pin-32));  
    }
}


/* Turn LEDs on and off for a brief time.
 * TODO: Don't spin the CPU while waiting. */
void led_blink(void) {
    unsigned int ra;

    if (!is_initialized)
        return;

    set_led(RED_LED, ON);
    set_led(GREEN_LED, OFF);
    for (ra = 0; ra < 0x100000; ra++)
        dummy(ra);

    set_led(RED_LED, OFF);
    set_led(GREEN_LED, ON);
    for (ra = 0; ra < 0x100000; ra++)
        dummy(ra);
}