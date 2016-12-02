#include "teletext.h"
#include <stdio.h>

// this char function assumes that each character is 5 bits across
// and 9 bits down
void writechar(int x_loc, int y_loc, char * ch) {
    unsigned char image[9];
    int r, c;

    // initialize image from the teletext
    for (r = 0; r < 9; r++)
        image[r] = teletext[*ch - 32][r];

    for (c = 0; c < 9; c++) {
        for (r = 4; r >= 0; r--) {
            if (image[c] & (1 << r))
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}

int main(void) {
    char *ch = "Hello World";
    writechar(0, 0, ch);

    return 0;
}
