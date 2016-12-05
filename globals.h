/* globals.h
 *
 * Description: Type definitions, macros, and other
 * code that's relevant to the rest of the project.
 */

#ifndef GLOBALS_H
#define GLOBALS_H


/* "Null" reference value, for convenience */
#define NULL ((void *) 0)

/* Standard "Boolean" type, for convenience */
typedef enum {
    FALSE,
    TRUE
} bool_t;


/* Method exit return values */
enum {
    SUCCESS,
    FAILURE
};


/* Forward declare our assembly methods */
extern void PUT32 (unsigned int dst_addr, unsigned int data);
extern void PUT16 (unsigned int dst_addr, unsigned int data);
extern void PUT8  (unsigned int dst_addr, unsigned int data);
extern unsigned int GET32 (unsigned int src_addr);
extern unsigned int GETPC (void);
extern void dummy (unsigned int dummy_arg);


#endif
