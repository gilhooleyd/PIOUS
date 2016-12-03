/* globals.h
 *
 * Description: Type definitions, macros, and other
 * code that's relevant to the rest of the project.
 */

#ifndef GLOBALS_H
#define GLOBALS_H


/* Access to our assembly methods */
extern void PUT32 (unsigned int, unsigned int);
extern void PUT16 (unsigned int, unsigned int);
extern void PUT8  (unsigned int, unsigned int);
extern unsigned int GET32 (unsigned int);
extern unsigned int GETPC (void);
extern void dummy (unsigned int);

/* "Null" reference value, for convenience */
#define NULL ((void *) 0)

/* Standard "Boolean" type, for convenience */
typedef enum {
    FALSE,
    TRUE
} bool_t;

/* Shorthand for commonly used types */
typedef unsigned int uint;
typedef unsigned long ulong;


#endif
