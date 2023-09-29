/**************************************************************
*
*                     unblackedges.c
*
*     Assignment: iii
*     Authors:  Josh Field (jfield02) and Vedant Modi (vmodi01)
*     Date:     28 September 2023
*
*     Summary:
*     unblackedges takes in a pbm (bitmap) (either through a file or stdin) 
*     and removes the black edges from the bitmap. It prints the fixed image
*     to stdout.
*
**************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include "bit2.h"
#include <pnmrdr.h>
#include <assert.h>
#include <set.h>
#include <mem.h>
#include <stack.h>

#define BLACK 1
#define WHITE 0

struct coords {
        int x;
        int y;
};
typedef struct coords *coords;

Bit2_T pbmread(FILE *inputfp);
void pbmwrite(Bit2_T bitmap);
void check_pixels(int x, int y, Bit2_T image, int elem, void *cl);
bool is_edge(int x, int y, Bit2_T image);
void check_neighbors(Stack_T neighbors, Bit2_T image);
bool in_range(coords curr_coord, Bit2_T image);
void print_pixel(int x, int y, Bit2_T image, int elem, void *cl);
coords coords_new(int x, int y);



/********** main ********
 *
 * Removes black edges from a scanned image
 *
 * Parameters:
 *      int argc: Number of arguments
 *      char *argv[]: Array of arguments
 * Return: EXIT_SUCCESS if the provided sudoku solution is valid, EXIT_FAILURE 
 *         otherwise
 *
 * Expects
 *      argc to be 1 or 2
 *      File to be a valid file, or input via stdin to be given
 *      check_pixels is not NULL
 * Notes:
 *      Will CRE if a provided file fails to open
 ************************/
int main(int argc, char *argv[])
{
        assert(argc == 1 || argc == 2);
        FILE *fp;
        if (argc > 1) {
                fp = fopen(argv[1], "r");
                assert(fp != NULL); /* CRE: File failed to open */
        } else {
                fp = stdin;
        }

        /* gather image into Bit2_T format */
        Bit2_T image = pbmread(fp);
        
        assert(check_pixels != NULL); /* CRE if check_pixels is NULL */

        /* replace black edges in Bit2 */
        Bit2_map_row_major(image, check_pixels, NULL);

        /* write bit2_T back to stdout in the format of a P1 */
        pbmwrite(image);

        Bit2_free(&image);

        fclose(fp);    
        return EXIT_SUCCESS;
}

/********** pbmread ********
 *
 * Reads an image file into a bitmap
 *
 * Parameters:
 *      FILE *fp: A file stream containing a bitmap of an image
 *    
 * Return: A Bit2_T representing the image provided
 *
 * Expects
 *      FILE *fp has been opened successfully
 *      bitmap in file has a non-zero height and width
 * Notes
 *      Will CRE if fp is NULL
 ************************/
Bit2_T pbmread(FILE *inputfp)
{
        assert(inputfp != NULL); /* Will CRE if inputfp is NULL */
        /* initialize a pnmrdr and bit2 */
        Pnmrdr_T rdr = Pnmrdr_new(inputfp);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);
        Bit2_T image = Bit2_new(data.width, data.height);

        /* CRE if width or height in P1 is 0 */
        assert(data.width != 0 || data.height != 0);

        /* populate the Bit2 */
        for (int i = 0; i < (int)data.height; i++) {
                for (int j = 0; j < (int)data.width; j++) {
                        unsigned val = Pnmrdr_get(rdr);
                        Bit2_put(image, j, i, (int)val);
                }
        }

        Pnmrdr_free(&rdr);
        return image;
}

/********** pbmwrite ********
*
* Writes the given image to stdout
*
* Parameters: 
*        Bit2_T bitmap: bitmap to write out
*
* Return: nothing
*
* Expects:
*       bitmap is not NULL
*       print_pixel is not NULL
* Notes:
*       Will CRE if bitmap is NULL
*
************************/
void pbmwrite(Bit2_T bitmap) 
{
        /* CRE if bitmap is NULL */
        assert(bitmap != NULL);
        
        /* print the P1 header */
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);
        fprintf(stdout, "P1\n%d %d\n", width, height);

        /* traverse the bit2 in row major order to print the image */
        assert(print_pixel != NULL); /* CRE if print_pixel is NULL */
        Bit2_map_row_major(bitmap, print_pixel, NULL); 
}

/********** print_pixel ********
*
* Prints one pixel in a P1 grayscale image
*
* Parameters: 
*        int x: Col index of the pixel
*        int y: Row index of the pixel
*        Bit2_T image: Bitmap of the image
*        int elem: element
*
* Return: Nothing
*
* Expects:
*      image to not be NULL
* Notes:
*      Will CRE if image is NULL
*
************************/
void print_pixel(int x, int y, Bit2_T image, int elem, void *cl)
{
        assert(image != NULL); /* CRE if image is NULL */
        (void)y;
        (void)cl;
        
        printf("%d", elem); 
        
        /* Print newline iff last elem in row */
        if(x == Bit2_width(image) - 1) { 
                printf("\n");
        }
}

/********** check_pixels ********
*
* Will clear given pixel in image if black edge, along with its touching 
* neighbor pixels
*
* Parameters: 
*        int x: Col index of the pixel
*        int y: Row index of the pixel
*        Bit2_T image: Bitmap of the image
*        int elem: value of the bitmap at the pixel
*        void *cl: Closure obtained through traversing through each pixel,
*                  unused here
*
* Return: nothing
*
* Expects:
*      Expects image to be not null
* Notes:
*      CRE if image is null
*
************************/
void check_pixels(int x, int y, Bit2_T image, int elem, void *cl)
{
        (void)elem;
        (void)cl;
        assert(image != NULL); /* CRE if image is NULL */
        if (is_edge(x, y, image)) {
                Stack_T neighbors = Stack_new();
                /* CRE if stack is NULL */
                assert(neighbors != NULL); 

                coords curr_coords = coords_new(x, y);

                Stack_push(neighbors, curr_coords);

                check_neighbors(neighbors, image);

                Stack_free(&neighbors);
        }
        
}

/********** is_edge ********
 *
 * Finds if a pixel is an edge pixel in the given bitmap
 *
 * Parameters:
 *      int x: col coordinate of pixel
 *      int y: row coordinate of pixel
 *      Bit2_T image: A Bit2 representation of the bitmap
 * Return: true if the given index of the bitmap is an edge, false if not
 *
 * Expects
 *      image may not be NULL
 * Notes:
 *      Will CRE if image is NULL
 ************************/
bool is_edge(int x, int y, Bit2_T image)
{
        assert(image != NULL); /* CRE if image is NULL */
        return x == 0 || y == 0 || x == Bit2_width(image) - 1
                                || y == Bit2_height(image) - 1;
}

/********** check_neighbors ********
 *
 * Checks the pixel and its neighbors to see if they are black edges, swapping 
 * them for white pixels instead, in a given bitmap
 *
 * Parameters:
 *      Stack_T neighbors: A stack of coords of neighbors
 *      Bit2_T image: A Bit2 representation of the bitmap
 * Return: true if the given index of the bitmap is an edge, false if not
 *
 * Expects
 *      image may not be NULL
 * Notes:
 *      Will CRE if image is NULL
 ************************/
void check_neighbors(Stack_T neighbors, Bit2_T image)
{
        /* Will CRE if neighbors_x, image is NULL */
        assert(neighbors != NULL || image != NULL); 
        while(!Stack_empty(neighbors)) {
                /* Pops from top of stack */
                coords curr_coord = Stack_pop(neighbors); 
                int curr_col = curr_coord->x;
                int curr_row = curr_coord->y;

                /* If the neighbor exists and is black, find its neighbors */
                if (in_range(curr_coord, image) 
                    && Bit2_get(image, curr_col, curr_row) == BLACK) {
                        
                        /* initialize four coords for each neighbor*/
                        coords neighbor_up = 
                                        coords_new(curr_col, curr_row + 1);
                        coords neighbor_down = 
                                        coords_new(curr_col, curr_row - 1);
                        coords neighbor_right = 
                                        coords_new(curr_col + 1, curr_row);
                        coords neighbor_left = 
                                        coords_new(curr_col - 1, curr_row);

                        /* push each neighbor to the stack */
                        Stack_push(neighbors, neighbor_up);
                        Stack_push(neighbors, neighbor_down);
                        Stack_push(neighbors, neighbor_right);
                        Stack_push(neighbors, neighbor_left);

                        /* Change the black pixel to white */
                        Bit2_put(image, curr_col, curr_row, WHITE); 
                } 

                FREE(curr_coord);
        }
}

/********** coords_new ********
*
* Makes a new set of coordinates and returns the ptr to the set
*
* Parameters: 
*        int x: Col index of coordinate
*        int y: Row index of coordinate
*
* Return: Ptr to coords set newly created
*
* Expects:
*      Nothing
* Notes:
*      Will CRE if alloc failed
*
************************/
coords coords_new(int x, int y)
{
        coords curr_coords;
        NEW0(curr_coords);

        curr_coords->x = x;
        curr_coords->y = y;
        
        return curr_coords;
}


/********** in_range ********
 *
 * Finds if a pixel coordinates exists in the given bitmap
 *
 * Parameters:
 *      coords curr_coord: Set of coordinate values to check
 *      Bit2_T image: A Bit2 representation of the bitmap
 * Return: true if the given index is in bitmap, false otherwise
 *
 * Expects
 *      curr_coord may not be NULL
 *      image may not be NULL
 * Notes:
 *      Will CRE if image is NULL
 *      Will CRE if curr_coord is NULL
 ************************/
bool in_range(coords curr_coord, Bit2_T image)
{
        /* CRE if image are NULL */
        assert(image != NULL && curr_coord != NULL);
        int height = Bit2_height(image);
        int width = Bit2_width(image);

        int x = curr_coord->x;
        int y = curr_coord->y;
        
        return x >= 0 && x < width && y >= 0 && y < height;
}

#undef BLACK
#undef WHITE