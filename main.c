/* main.c
 *
 * Description: Defines the kernel main function.
 */


#include "globals.h"
#include "framebuffer.h"
#include "led.h"
#include "screen.h"
#include "tty.h"

#include "image_data.h"
#include "teletext.h"


/* Main function for the kernel - never returns */
void kernel_main(void)
{
    int num_blinks;

    // Initialize modules
    terminal_initialize();
    led_init();

    terminal_writestring("Hello World!");

    // Blink a bunch
    num_blinks = 20;
    while (num_blinks-- > 0)
        led_blink();

    // Hang forever
    while(TRUE);
}
