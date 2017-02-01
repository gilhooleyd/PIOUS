#ifndef PS2_H 
#define PS2_H 

#ifdef ARCH_X86
void PS2_init();
int  PS2_readChar();
#else
inline void PS2_init() {};
inline int  PS2_readChar() {return -1;};
#endif

#endif
