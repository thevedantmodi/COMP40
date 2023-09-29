/**************************************************************
 *
 *                     restoration.c
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01) 
 *     Date:     10 Sep 2023
 *
 *     Summary:
 *     restoration takes in a corrupted pgm image file, either through a file
 *     name provided on the command line, or through stdin. It parses the file,
 *     determines which lines are original, and outputs a restored P5 pgm
 *     (binary) image to stdout.
 *
 **************************************************************/
 
#include "file_handler.h"
#include "free_mem.h"
#include <stdlib.h>


#define SEQ_STRT_CAPAC 1000

void output_pgm(Seq_T *data, int height);

/**********main********
 *
 * Runs the restoration program from file in command or stdin
 * Inputs:
 *      int argc: The amount of arguments provided to the program
 *      char *argv: Array of arguments provided
 * Return: 0 on Success
 * Expects
 *      If file called, it's valid
 *      A file or stdin
 ************************/
int main(int argc, char *argv[])
{
        FILE *fp;
        if (argc > 1) {
                assert(argc == 2); /* c.r.e: Wrong number of args provided */
                fp = fopen(argv[1], "r");
                assert(fp != NULL); /* c.r.e: File failed to open */
        } else {
                fp = stdin;
        }

        Seq_T original_rows = Seq_new(SEQ_STRT_CAPAC);

        int height = readfile(fp, &original_rows);
        fclose(fp);

        output_pgm(&original_rows, height);

        free_Sequence(&original_rows);

        return EXIT_SUCCESS;
}

/* output_pgm
 * Outputs a pgm image in P5 format to stdout from image data stored in a Hanson
 * Sequence
 * Inputs:
 * Seq_T *data: a pointer to the Seq that contains the image data
 * int height: the height of the image to be printed
 * Returns: None
 * Expects: data is nonnull and *data is populated with integer pointers,
 *          height is positive int, the length of *data is divisible by height
 * Notes: Output is printed to stdout
 */
void output_pgm(Seq_T *data, int height)
{
        /* CRE if image is invalid (less than 2 lines, or inconsistent amount
        of numbers per line) */
        assert(height >= 2);
        assert(Seq_length(*data) % height == 0);

        /* Print image header to stdout */
        int width = Seq_length(*data) / height;
        printf("P5\n%d %d\n255\n", width, height);

        /* Print image data in binary form */
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        int seq_index = (i * width) + j;
                        putchar(*((int *)Seq_get(*data, seq_index)));
                }
        }
}

#undef SEQ_STRT_CAPAC