/*
 * uarray2_pnm.c
 * Josh Field (jfield02) and Vedant Modi (vmodi01)
 *
 * 25 Sep 2023
 * 
 * Tests a UArray2 with PNM images
 */

#include <pnmrdr.h>
#include <stdio.h>
#include <stdlib.h>
#include "uarray2.h"

void print_image(int x, int y, UArray2_T image, void *elem, void *cl) 
{
        int *entry_p = elem;

        /* Did at work correctly? */
        *((bool *)cl) &= UArray2_at(image, x, y) == entry_p;

        /* printf("ar[%d,%d]\n", i, j); */
        printf("%d", *entry_p);

        if (x == 49) {
                printf("\n");
        }
}

int main(int argc, char *argv[]) 
{
        (void)argc;
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL) {
                fprintf(stderr,
                        "%s: %s %s %s\n",
                        argv[0], "Could not open file ",
                        argv[1], "for reading");
                exit(1);
        }
        Pnmrdr_T rdr = Pnmrdr_new(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        UArray2_T image = UArray2_new(data.width, data.height, sizeof(int));


        for (int i = 0; i < (int)data.height; i++) {
                for (int j = 0; j < (int)data.width; j++) {
                        unsigned val = Pnmrdr_get(rdr);
                        *((int *)UArray2_at(image, j, i)) = (int)val;
                }
        }

        Pnmrdr_free(&rdr);
        bool OK = true;
        fprintf(stderr, "Printing image");
        printf("P1\n50 50\n");
        UArray2_map_row_major(image, print_image, &OK);

        fclose(fp);
        return 0;
}
