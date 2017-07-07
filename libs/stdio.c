#include "stdio.h"

void printDigit(int dig){ 
    char codes[] = "0123456789ABCDEF";
    mainPrintChar(codes[dig]);
}

void printHex(int num) {
    if (num < 0x10) {
        printDigit(num); 
        return;
    }
    else {
        printHex   (num / 0x10);
        printDigit (num % 0x10);
    }    
}

void printNum(int num) {
    if (num < 10) {
        printDigit(num); 
        return;
    }
    else {
        printNum  (num / 10);
        printDigit(num % 10);
    }    
}
