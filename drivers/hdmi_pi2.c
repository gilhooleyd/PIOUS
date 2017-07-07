/* david.c
 *
 * (Lance) This file is the video01.c file from the old
 * master branch. I included it here so David wouldn't get mad.
 */

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "teletext.h"
#include "tty.h" 
#include "barrier.h" 
#include "uart.h" 
#include "framebuffer.h" 

#define MB_ADDR 0x3F00B880
#define MB_STATUS_OFF 0x18
#define MB_READ_OFF   0x00
#define MB_WRITE_OFF  0x20
#define MB_IS_EMPTY   0x40000000
#define MB_IS_FULL    0x80000000

#define MB_TYPE_FB 0x1

unsigned int toVCAddr( unsigned int addr) {
    return addr + 0x40000000;
}

unsigned int fromVCAddr( unsigned int addr) {
    return addr & 0x3FFFFFFF;
}

unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox= MB_ADDR;
    while(1)
    {
        if((GET32(mailbox+MB_STATUS_OFF) & MB_IS_FULL)==0) break;
    }
    dmb();
    PUT32(mailbox+MB_WRITE_OFF,fbinfo_addr+channel);
    return(0);
}

unsigned int MailboxRead ( unsigned int channel )
{
    unsigned int ra;
    unsigned int mailbox;

    mailbox = MB_ADDR;
    while(1)
    {
        while(1)
        {
            dmb();
            ra = GET32(mailbox + MB_STATUS_OFF);
            dmb();

            if ((ra & MB_IS_EMPTY) == 0) break;
        }
        ra = GET32(mailbox + MB_READ_OFF);
        if ((ra&0xF) == channel) break;
    }
    return(ra);
}

#define HEIGHT 480
#define WIDTH  640 

#define GPFSEL3        0x3F20000C
#define GPFSEL4        0x3F200010
#define GPSET1         0x3F200020
#define GPCLR1         0x3F20002C

#define MB_STRUCT_ADDR 0x00040000

#define CHARSIZE_X 6
#define CHARSIZE_Y 10
#define COLOR_BLACK 0x00000000
#define COLOR_WHITE 0xFFFFFFFF

bool_t terminal_init(struct fb_screen_t *fb) {
    unsigned int rb;
    unsigned int mb_addr = MB_STRUCT_ADDR;

    mb_addr = MB_STRUCT_ADDR;
    // create the structure to be passed to the mailbox
    PUT32(mb_addr, WIDTH); /* #0 Physical Width */
    mb_addr += 4;
    PUT32(mb_addr, HEIGHT);  /* #4 Physical Height */
    mb_addr += 4;
    PUT32(mb_addr, WIDTH); /* #8 Virtual Width */
    mb_addr += 4;
    PUT32(mb_addr, HEIGHT);  /* #12 Virtual Height */
    mb_addr += 4;
    PUT32(mb_addr, 0);      /* #16 GPU - Pitch */
    mb_addr += 4;
    PUT32(mb_addr, 32);     /* #20 Bit Depth */
    mb_addr += 4;
    PUT32(mb_addr, 0);      /* #24 X */
    mb_addr += 4;
    PUT32(mb_addr, 0);      /* #28 Y */
    mb_addr += 4;
    PUT32(mb_addr, 0);      /* #32 GPU - Pointer */
    mb_addr += 4;
    PUT32(mb_addr, 0);      /* #36 GPU - Size */

    // set address back to normal
    mb_addr = MB_STRUCT_ADDR;


    // pass the structure to the Framebuffer mailbox
    MailboxWrite(toVCAddr(mb_addr), MB_TYPE_FB);
    MailboxRead(1);

    // read the location of the framebuffer
    rb = GET32(mb_addr + 0x20);
    rb = rb & 0x3FFFFFFF;
    fb->info.fb_addr = rb;

    fb->info.fb_width = WIDTH;
    fb->info.fb_height = HEIGHT;
    fb->write_x = 0;
    fb->write_y = 0;
    fb->font_size = 1;

    return(0);
}

bool_t terminal_is_initialized(struct fb_screen_t *fb) {return FALSE;}
