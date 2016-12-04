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
enum
{
    // GPIO function select pin addresses
    GPFSEL3 = 0x3F20000C,
    GPFSEL4 = 0x3f200010,

    // GPIO pin set and clear addresses
    GPSET1  = 0x3F200020,
    GPCLR1  = 0x3F20002C,
};


/* LED methods */
bool_t led_init(void);
bool_t led_is_initialized(void);

void led_blink(void);


#endif
