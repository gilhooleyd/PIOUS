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
bool_t led_init(void)
{
    unsigned int ra;

    // TODO: Someone else please explain what's going
    // on, or re-write to make more sense :(

    ra=GET32(GPFSEL4);
    ra&=~(7<<21);
    ra|=1<<21;
    PUT32(GPFSEL4,ra);

    ra=GET32(GPFSEL3);
    ra&=~(7<<15);
    ra|=1<<15;
    PUT32(GPFSEL3,ra);

    is_initialized = TRUE;
    return is_initialized;
}


/* Returns whether this module was initialized */
bool_t led_is_initialized(void)
{
    return is_initialized;
}


/* Turns the LED on, then back off after a brief time. *
 *                                                     *
 * TODO: Currently, this spins the CPU while waiting.  *
 * Eventually, this shouldn't prevent other things     *
 * from happening in the mean time.                    */
void led_blink(void)
{
    unsigned int ra;

    if (!is_initialized)
        return;

    // TODO: Someone else please explain what's going
    // on, or re-write to make more sense :(

    // Turn the LED on and wait
    PUT32(GPSET1,1<<(47-32));
    PUT32(GPCLR1,1<<(35-32));
    for(ra=0;ra<0x100000;ra++)
        dummy(ra);

    // Turn the LED off and wait
    PUT32(GPCLR1,1<<(47-32));
    PUT32(GPSET1,1<<(35-32));
    for(ra=0;ra<0x100000;ra++)
        dummy(ra);
}
