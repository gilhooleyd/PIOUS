/* framebuffer.c
 *
 * Description: Module for interfacing with Pi 2's framebuffer.
 */


#include "framebuffer.h"
#include "mbox.h"


/* Whether this module is initialized */
static bool_t is_initialized;

/* Pointer to the frame buffer info structure */
static fb_info *fb_info_ptr;

/* Frame buffer address */
static unsigned int fb_addr;


/* Initializes the frame buffer module.      *
 * Must be called before all other methods.  *
 * Returns whether initialization succeeded. */
bool_t fb_init(void)
{
    unsigned int fb_info_addr;

    if (is_initialized)
        return TRUE;

    // Set the info pointer to the designated spot in memory
    fb_info_addr = FB_INFO_ADDR;
    fb_info_ptr  = (fb_info *) fb_info_addr;

    // Fill the info structure with data
    fb_info_ptr->width = FB_WIDTH;
    fb_info_ptr->height = FB_HEIGHT;
    fb_info_ptr->virtual_width = FB_WIDTH;
    fb_info_ptr->virtual_height = FB_HEIGHT;
    fb_info_ptr->pitch = 0;
    fb_info_ptr->depth = 32;
    fb_info_ptr->x_offset = 0;
    fb_info_ptr->y_offset = 0;
    fb_info_ptr->fb_addr = 0;
    fb_info_ptr->size = 0;

    // Write info structure address to framebuffer channel
    mbox_write(MBOX_FB_CHANNEL, fb_info_addr);

    // Wait for GPU to modify the structure
    mbox_read(MBOX_FB_CHANNEL);

    // Read the frame buffer location (must subtract VC's
    // start address to get the actual memory location)
    fb_addr = fb_info_ptr->fb_addr - VC_START_ADDR;

    // If frame buffer address was modified, initialization successful
    is_initialized = (fb_addr != 0);
    return is_initialized;
}


/* Returns whether the module is initialized */
bool_t fb_is_initialized(void)
{
    return is_initialized;
}


/* Returns the modified frame buffer info structure */
fb_info *fb_get_info(void)
{
    if (!is_initialized)
        return NULL;
    return fb_info_ptr;
}


/* Returns the address of the requested frame buffer */
unsigned int fb_get_addr(void)
{
    if (!is_initialized)
        return -1;
    return fb_addr;
}

