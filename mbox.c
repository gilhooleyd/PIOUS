/* mbox.h
 *
 * Description: Library for Pi 2 mailbox interfacing.
 * Current version only supports reading/writing to mailbox 0.
 */


#include "mbox.h"


/* Returns the value in mailbox 0 on the specified channel */
unsigned int mbox_read(unsigned int channel)
{
    unsigned int status;
    unsigned int data;

    while(TRUE)
    {
        // Wait until mailbox is not empty
        while(TRUE)
        {
            status = GET32(MBOX_ADDR + MBOX0_STATUS_OFFSET);
            if (!(status & MBOX_IS_EMPTY))
                break;
        }

        // Wait until data returned by the mailbox has
        // correct channel stored in the lower 4 bits
        // (Actual return data stored in upper 28 bits)
        data = GET32(MBOX_ADDR + MBOX0_READ_OFFSET);
        if ((data & 0xF) == channel)
            break;
    }

    return data;
}


/* Writes the provided data into mailbox 0 on *
 * the specified channel. Only the upper 28   *
 * bits of data should contain relevant info. */
void mbox_write(unsigned int channel, unsigned int data)
{
    unsigned int status;
    unsigned int write_data;

    // Wait until mailbox is not full
    while(TRUE)
    {
        status = GET32(MBOX_ADDR + MBOX0_STATUS_OFFSET);
        if (!(status & MBOX_IS_FULL))
            break;
    }

    // Write the data into the mailbox:
    // Channel is stored in lower 4 bits,
    // Actual data address in upper 28 bits
    write_data = (data & 0xFFFFFFF0) + channel;
    PUT32(MBOX_ADDR + MBOX0_WRITE_OFFSET, write_data);
}
