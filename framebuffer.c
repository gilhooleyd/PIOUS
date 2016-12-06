/* framebuffer.c
 *
 * Description: Module for interfacing with Pi 2's framebuffer.
 */


#include "framebuffer.h"
#include "mbox.h"


/* Whether this module is initialized */
static bool_t is_initialized;

/* Frame buffer info structure */
static fb_info_t fb_info;

/* Frame buffer address */
static unsigned int fb_addr;


/* Initializes the frame buffer module.      *
 * Must be called before all other methods.  *
 * Returns whether initialization succeeded. */
bool_t fb_init(void)
{
    unsigned int fb_info_addr;

    // Get the address of the fb info struct
    fb_info_addr = (unsigned int) &fb_info;

    // Fill the info structure with data
    fb_info.width = FB_WIDTH;
    fb_info.height = FB_HEIGHT;
    fb_info.virtual_width = FB_WIDTH;
    fb_info.virtual_height = FB_HEIGHT;
    fb_info.pitch = 0;
    fb_info.depth = 32;
    fb_info.x_offset = 0;
    fb_info.y_offset = 0;
    fb_info.fb_addr = 0;
    fb_info.size = 0;

    // Write info structure address to framebuffer channel
    // (Need to add VC_START_ADDR for video core address)
    mbox_write(MBOX_FB_CHANNEL, fb_info_addr + VC_START_ADDR);

    // Wait for GPU to modify the structure
    mbox_read(MBOX_FB_CHANNEL);

    // Read the frame buffer location
    // (Need to subtract VC_START_ADDR since it came from video core)
    fb_addr = fb_info.fb_addr - VC_START_ADDR;

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
fb_info_t *fb_get_info(void)
{
    if (!is_initialized)
        return NULL;
    return &fb_info;
}


/* Returns the address of the requested frame buffer */
unsigned int fb_get_addr(void)
{
    if (!is_initialized)
        return -1;
    return fb_addr;
}

