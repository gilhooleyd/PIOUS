/* mbox.h
 *
 * Description: Library for Pi 2 mailbox interfacing.
 * Current version only supports reading/writing to mailbox 0.
 */

#ifndef MBOX_H
#define MBOX_H


#include "globals.h"


/* Constants */
enum
{
    // Bus address for mailbox memory mapped registers on Pi 2
    MBOX_ADDR   = 0x3F00B880,

    // Bit 30 is set in status register if read mailbox is full
    MBOX_IS_EMPTY   = 0x40000000,

    // Bit 31 is set in status register if write mailbox is full
    MBOX_IS_FULL    = 0x80000000,

    // Mailbox frame buffer channel
    MBOX_FB_CHANNEL = 0x1,

    // Status register offset for mailbox 0
    MBOX0_STATUS_OFFSET = 0x18,

    // Read register offset for mailbox 0
    MBOX0_READ_OFFSET   = 0x00,

    // Write register offset for mailbox 0
    MBOX0_WRITE_OFFSET  = 0x20,
};


/* Mailbox methods */
unsigned int mbox_read(unsigned int channel);
void mbox_write(unsigned int channel, unsigned int data);


#endif
