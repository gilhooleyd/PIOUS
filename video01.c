// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "image_data.h"

#define MB_ADDR 0x3F00B880
#define MB_STATUS_OFF 0x18
#define MB_READ_OFF   0x00
#define MB_WRITE_OFF  0x20
#define MB_IS_EMPTY   0x40000000
#define MB_IS_FULL    0x80000000

#define MB_TYPE_FB 0x1

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );
extern void dummy (unsigned int);

unsigned int toVCAddr( unsigned int addr) {
    return addr; // + 0x40000000;
}

unsigned int fromVCAddr( unsigned int addr) {
    return addr - 0x40000000;
}

unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox= MB_ADDR;
    while(1)
    {
        if((GET32(mailbox+MB_STATUS_OFF) & MB_IS_FULL)==0) break;
    }
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
            ra = GET32(mailbox + MB_STATUS_OFF);
            if ((ra & MB_IS_EMPTY) == 0) break;
        }
        ra = GET32(mailbox + MB_READ_OFF);
        if ((ra&0xF) == channel) break;
    }
    return(ra);
}

#define HEIGHT 640 
#define WIDTH  480 

#define GPFSEL3 0x3F20000C
#define GPFSEL4 0x3F200010
#define GPSET1  0x3F200020
#define GPCLR1  0x3F20002C

#define MB_STRUCT_ADDR 0x00040000

int notmain ( void )
{
    unsigned int ra,rb;
    unsigned int ry,rx;
    unsigned int mb_addr = MB_STRUCT_ADDR;

    int blink = 20;

    ra=GET32(GPFSEL4);
    ra&=~(7<<21);
    ra|=1<<21;
    PUT32(GPFSEL4,ra);

    ra=GET32(GPFSEL3);
    ra&=~(7<<15);
    ra|=1<<15;
    PUT32(GPFSEL3,ra);


    mb_addr = MB_STRUCT_ADDR;
    // create the structure to be passed to the mailbox
    PUT32(mb_addr, HEIGHT); /* #0 Physical Width */
    mb_addr += 4;
    PUT32(mb_addr, WIDTH);  /* #4 Physical Height */
    mb_addr += 4;
    PUT32(mb_addr, HEIGHT); /* #8 Virtual Width */
    mb_addr += 4;
    PUT32(mb_addr, WIDTH);  /* #12 Virtual Height */
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
    mb_addr = MB_STRUCT_ADDR;


    // pass the structure to the Framebuffer mailbox
    MailboxWrite(toVCAddr(mb_addr), MB_TYPE_FB);
    MailboxRead(1);

    // read the location of the framebuffer
    rb = GET32(mb_addr + 0x20);
    rb = fromVCAddr(rb);

    // write the image
    ra=0;
    for (ry = 0; ry < HEIGHT; ry++)
    {
        for (rx = 0; rx < WIDTH; rx++)
        {
            PUT32(rb, image_data[ra++]);
            rb += 4;
        }
    }

    // blink a bunch
    while (blink-- > 0 )
    {
        PUT32(GPSET1,1<<(47-32));
        PUT32(GPCLR1,1<<(35-32));
        for(ra=0;ra<0x100000;ra++) dummy(ra);
        PUT32(GPCLR1,1<<(47-32));
        PUT32(GPSET1,1<<(35-32));
        for(ra=0;ra<0x100000;ra++) dummy(ra);
    }

    return(0);
}
