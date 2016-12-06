/* main.c
 *
 * Description: Defines the kernel main function.
 */


#include "globals.h"
#include "framebuffer.h"
#include "led.h"
#include "screen.h"

#include "image_data.h"
#include "teletext.h"


/* Main function for the kernel - never returns */
void kernel_main(void)
{
    unsigned int pixel_addr;
    int num_blinks;

    // Initialize modules
    fb_init();
    led_init();

    // Get the start address of the frame buffer
    pixel_addr = fb_get_addr();

    // Blink a bunch
    num_blinks = 20;
    while (num_blinks-- > 0)
        led_blink();

    // Hang forever
    while(TRUE);
}
