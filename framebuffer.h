/* framebuffer.h
 *
 * Description: Module for interfacing with Pi 2's framebuffer.
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


#include "globals.h"


/* Constants */
enum
{
    // Start address of physical memory as seen by the Video Core
    VC_START_ADDR   = 0x40000000,

    // Mailbox frame buffer channel
    MBOX_FB_CHANNEL = 0x1,

    // Frame buffer width in pixels
    FB_WIDTH    = 640,

    // Frame buffer height in pixels
    FB_HEIGHT   = 480,
};


/* Defines the parameters of the frame buffer. *
 * Gets passed to and modified by the GPU.     */
struct fb_info_t
{
    // Width of the requested frame buffer
    unsigned int width;

    // Height of the requested frame buffer
    unsigned int height;

    // Virtual width of frame buffer (for rescaling)
    unsigned int virtual_width;

    // Virtual height of frame buffer (for rescaling)
    unsigned int virtual_height;

    // Number of bytes between each row of the frame buffer (set by GPU)
    unsigned int pitch;

    // Number of bits per pixel of frame buffer
    unsigned int depth;

    // Offset in the x direction (for repositioning)
    unsigned int x_offset;

    // Offset in the y direction (for repositioning)
    unsigned int y_offset;

    // Pointer to the frame buffer (set by GPU)
    unsigned int fb_addr;

    // Size of the frame buffer (set by GPU)
    unsigned int size;
};
typedef struct fb_info_t fb_info_t;


/* Frame buffer methods */
bool_t fb_init(void);
bool_t fb_is_initialized(void);

fb_info_t *fb_get_info(void);
unsigned int fb_get_addr(void);


#endif
