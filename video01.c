
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

// 2  outer corner
// 4
// 6
// 8  TX out
// 10 RX in

// The raspberry pi firmware at the time this was written defaults
// loading at address 0x8000.  Although this bootloader could easily
// load at 0x0000, it loads at 0x8000 so that the same binaries built
// for the SD card work with this bootloader.  Change the ARMBASE
// below to use a different location.

#include "image_data.h"

extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern unsigned int GETPC ( void );

unsigned int MailboxWrite ( unsigned int fbinfo_addr, unsigned int channel )
{
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        if((GET32(mailbox+0x18)&0x80000000)==0) break;
    }
    PUT32(mailbox+0x20,fbinfo_addr+channel);
    return(0);
}

unsigned int MailboxRead ( unsigned int channel )
{
    unsigned int ra;
    unsigned int mailbox;

    mailbox=0x2000B880;
    while(1)
    {
        while(1)
        {
            ra=GET32(mailbox+0x18);
            if((ra&0x40000000)==0) break;
        }
        ra=GET32(mailbox+0x00);
        if((ra&0xF)==channel) break;
    }
    return(ra);
}


//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra,rb;
    unsigned int ry,rx;

    PUT32(0x40040000, 640); /* #0 Physical Width */
    PUT32(0x40040004, 480); /* #4 Physical Height */
    PUT32(0x40040008, 640); /* #8 Virtual Width */
    PUT32(0x4004000C, 480); /* #12 Virtual Height */
    PUT32(0x40040010, 0);   /* #16 GPU - Pitch */
    PUT32(0x40040014, 32);  /* #20 Bit Depth */
    PUT32(0x40040018, 0);   /* #24 X */
    PUT32(0x4004001C, 0);   /* #28 Y */
    PUT32(0x40040020, 0);   /* #32 GPU - Pointer */
    PUT32(0x40040024, 0);   /* #36 GPU - Size */


    MailboxWrite(0x40040000,1);
    MailboxRead(1);

    rb=GET32(0x40040020);
    ra=0;
    for(ry=0;ry<480;ry++)
    {
        for(rx=0;rx<480;rx++)
        {
            PUT32(rb,image_data[ra++]);
            rb+=4;
        }
        for(;rx<640;rx++)
        {
            PUT32(rb,0);
            rb+=4;
        }
    }

    return(0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
