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

#define HEIGHT 480
#define WIDTH  640 

#define GPFSEL3        0x3F20000C
#define GPFSEL4        0x3F200010
#define GPSET1         0x3F200020
#define GPCLR1         0x3F20002C

#define MB_STRUCT_ADDR 0x00040000

#define CHARSIZE_X 6
#define CHARSIZE_Y 10
#define COLOR_BLACK 0x55550000
#define COLOR_WHITE 0xFFFFFFFF
static unsigned int screenbase;
static unsigned int fb_x, fb_y, pitch;
static unsigned char hello[5] = "Hello";
static unsigned char a[18] = 
    { 14,21,20,14,5,21,14,0,0,
	8,20,20,8,21,18,13,0,0 
};

// this char function assumes that each character is 5 bits across
// and 9 bits down
void writechar(int x_loc, int y_loc, int letter) { //char * ch) {
    unsigned int cursor = screenbase  
        + (x_loc * 5 * 4) 
        + (y_loc * 9 * fb_x * 4);
    unsigned char image[9];
    int r, c;

//    image[0] = 14;
//    image[1] = 21;
//    image[2] = 20;
//    image[3] = 14;
//    image[4] =  5;
//    image[5] = 21;
//    image[6] = 14;
//    image[7] =  0;
//    image[8] =  0;

    // initialize image from the teletext
    //
        for (r = 0; r < 9; r++) {
            int index = r + 9 * letter; 
            image[r] = a[index];
        }
//    if (letter == 1) {
//        for (r = 0; r < 9; r++) {
//            int index = r; 
//            image[r] = a[index];
//        }
//    } else {
//        for (r = 0; r < 9; r++) {
//            int index = r + 9; 
//            image[r] = a[index];
//        }
//    }

//        image[r] = teletext[*ch - 32][r];

    for (c = 0; c < 9; c++) {
        for (r = 4; r >= 0; r--) {
            if (image[c] & (1 << r))
                PUT32(cursor, COLOR_WHITE);
            else
                PUT32(cursor, COLOR_BLACK);
            cursor += 4;
        }
        cursor -= 4*5;
        cursor += 4*fb_x;
    }
}

int notmain ( void )
{
    unsigned int ra,rb;
    unsigned int ry,rx;
    unsigned int mb_addr = MB_STRUCT_ADDR;
    int i;

    int blink = 20;
    char *ch = "Hello World!";

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
    screenbase = rb;
    fb_x = WIDTH;
    fb_y = HEIGHT;

//    // write the image
//    ra=0;
//    int black = 1;
//    for (ry = 0; ry < HEIGHT; ry++)
//    {
//        for (rx = 0; rx < WIDTH; rx++)
//        {
//            if (black)
//                PUT32(rb, COLOR_BLACK);
//            else
//                PUT32(rb, COLOR_WHITE);
//            rb += 4;
//            black = 1 - black;
//        }
//    }

/*
    writechar(10, 10, 1);
    if (ch[0] == 'H')
        writechar(11, 10, 0);
    if (hello[0] == 'H')
        writechar(11, 14, 1);
 //   writechar(10, 10, ch);
 //   writechar(11, 10, &ch[1]);
 //   writechar(11, 14, &ch[2]);
*/

    // Print a string!
    for (i = 0; i < 5; i++)
    {
        PUT32(GPSET1,1<<(47-32));
        PUT32(GPCLR1,1<<(35-32));
        for(ra=0;ra<0x100000;ra++) dummy(ra);
        PUT32(GPCLR1,1<<(47-32));
        PUT32(GPSET1,1<<(35-32));
        for(ra=0;ra<0x100000;ra++) dummy(ra);

        writechar(10+i, 10, ch[i]);
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

