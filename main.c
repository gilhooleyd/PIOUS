/* main.c
 *
 * Description: Defines the kernel main function.
 */


#include "globals.h"
#include "framebuffer.h"
#include "led.h"

#include "image_data.h"
#include "teletext.h"


/* Main function for the kernel - never returns */
void kernel_main(void)
{
    unsigned int x, y;
    unsigned int pixel_addr;
    unsigned int pixel_index;
    int num_blinks;

    // Initialize modules
    fb_init();
    led_init();

    // Get the start address of the frame buffer
    pixel_addr = fb_get_addr();

    // Write the image
    pixel_index = 0;
    for (y = 0; y < FB_HEIGHT; y++)
    {
        for (x = 0; x < FB_WIDTH; x++)
        {
            PUT32(pixel_addr, image_data[pixel_index]);
            pixel_addr += 4;
            pixel_index++;
        }
    }

    // Blink a bunch
    num_blinks = 20;
    while (num_blinks-- > 0)
        led_blink();

    // Hang forever
    while(TRUE);
}
