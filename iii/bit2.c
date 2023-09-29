/**************************************************************
*
*                     bit2.c
*
*     Assignment: iii
*     Authors:  Josh Field (jfield02) and Vedant Modi (utln-2)
*     Date:     25 Sep 2023
*
*     Summary:
*     Bit2 implementation file, as defined in bit2.h. Uses a single Hanson
*     Bit structure to store each row of the Bit2 contiguously.
*
**************************************************************/
#include "bit2.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mem.h>

#define T Bit2_T

struct T {
        Bit_T array;
        int vertical;
        int horizontal;
};

/********** flattened_index ********
*
* Maps a 2-dimensional coordinate to a 1-dimensional point in the given Bit2
*
* Parameters: 
*        T bit2: The Bit2 structure in which the index is desired
*        int x: The 2-dimensional col index
*        int y: The 2-dimensional row index
*
* Return: The 1-dimensional index
*
* Expects:
*      bit2 to not be NULL
* Notes:
*      CRE if bit2 is NULL
*      
*       This function is most commonly used to get the one dimensional index
*       in the single Bit structure that stores the rows of the Bit2
*       contiguously.
*
************************/
int flattened_index(T bit2, int x, int y) 
{
        assert(bit2 != NULL); /* CRE if Bit2 is NULL */
        int height = Bit2_height(bit2);
        int width = Bit2_width(bit2);

        /* CRE for out of range element */
        assert(x <= width && x >= 0);
        assert(y <= height && y >= 0); 
        
        return (width * y) + x;
}

/********** Bit2_new ********
*
* Creates a new instance of the Bit2 structure
*
* Parameters: 
*        int width: Number of columns of desired Bit2
*        int height: Number of rows of desired Bit2
*
* Return: Newly created Bit2 instance
*
* Expects:
*       Dimensions of image to be non-zero positive integers
* Notes:
*       Will CRE if dimensions are not both non-zero positive integers
*
************************/
T Bit2_new(int width, int height)
{       
        T Bit2_p;
        NEW0(Bit2_p);

        assert(width >= 0 && height >= 0); /* CRE if negative dimensions */
        Bit2_p->vertical = height;
        Bit2_p->horizontal = width;

        int length = height * width;

        Bit2_p->array = Bit_new(length);
        assert(Bit2_p->array != NULL);

        return Bit2_p;
}

/********** Bit2_free ********
*
* Frees the Bit2 from memory
*
* Parameters: 
*        T *bit2: Ptr to Bit2 to free
*
* Return: Nothing
*
* Expects:
*       bit2 will not be NULL
*       *bit2 will not be NULL
* Notes:
*       Will CRE if either bit2 or *bit2 are NULL
************************/
void Bit2_free(T *bit2)
{
        assert(bit2 != NULL && *bit2 != NULL); /* CRE if Bit2 or *Bit2 NULL */
        /* Free the Bit before we free the overarching Bit2 */
        Bit_free(&((*bit2)->array)); 
        FREE(*bit2);
}

/********** Bit2_get ********
*
* Gets the bit at the given coordinates in the Bit2
*
* Parameters: 
*        T bit2: Bit2 to retrieve the value from
*        int x: Col index to retrieve the value from
*        int y: Row index to retrive the value from
*
* Return: Integer value at coordinate
*
* Expects:
*      bit2 to not be NULL
* Notes:
*      CRE if bit2 is NULL
*
************************/       
int Bit2_get(T bit2, int x, int y)
{
        assert(bit2 != NULL); /* CRE if Bit2 is NULL */
        return Bit_get(bit2->array, flattened_index(bit2, x, y));
}

/********** Bit2_put ********
*
* Places the bit specified at the given coords in the given Bit2 and returns
* the old value
*
* Parameters: 
*        T bit2: Bit2 to modify
*        int x: Col index to retrieve the value from
*        int y: Row index to retrive the value from
*        int bit: Bit to place in map
*
* Return: Old value in the map at the index
*
* Expects:
*      bit2 to not be NULL
* Notes:
*      CRE if bit2 is NULL
*
************************/
int Bit2_put(T bit2, int x, int y, int bit)
{
        assert(bit2 != NULL); /* CRE if Bit2 is NULL */
        return Bit_put(bit2->array, flattened_index(bit2, x, y), bit);
}

/********** Bit2_width ********
*
* Gives the number of columns of the image
*
* Parameters: 
*        T bit2: Bit2 instance to get the number of columns of
*
* Return: Number of columns in Bit2
*
* Expects:
*      bit2 is already initialized with Bit2_new
* Notes:
*      Will CRE if bit2 is NULL
*
************************/
int Bit2_width(T bit2)
{
        assert(bit2 != NULL); /* CRE if bit2 is NULL */
        return bit2->horizontal;
}

/********** Bit2_height ********
*
* Gives the number of rows of the image
*
* Parameters: 
*        T bit2: Bit2 instance to get the number of rows of
*
* Return: Number of rows in Bit2
*
* Expects:
*      bit2 is already initialized with Bit2_new
* Notes:
*      Will CRE if bit2 is NULL
*
************************/
int Bit2_height(T bit2)
{
        assert(bit2 != NULL); /* CRE if bit2 is NULL */
        return bit2->vertical;
}

/********** Bit2_map_col_major ********
 *
 * Traverses the bitmap where rows increase faster than columns, applying 
 * the provided function to each intialized index.
 *
 * Parameters:
 *      T bit2: the Bit2 of which the client desires a traversal of 
 *      void apply: The function that will be applied to each index
 *      bool *OK: Bool representing if the traversal succeeded
 *
 * Return: Nothing
 *
 * Expects
 *      Bit2 to be not NULL
 * Notes:
 *      Will CRE if Bit2 is NULL
 ************************/
void Bit2_map_col_major(T bit2, void apply(int x, int y, T bit2, 
                                        int elem, void *cl), bool *OK)
{
        assert(bit2 != NULL); /* CRE if Bit2 is NULL */
        int height = Bit2_height(bit2);
        int width = Bit2_width(bit2);
        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        apply(i, j, bit2, Bit2_get(bit2, i, j), OK);
                }
        }
}

/********** Bit2_map_row_major ********
 *
 * Traverses the bitmap where columns increase faster than rows, applying 
 * the provided function to each intialized index.
 *
 * Parameters:
 *      T bit2: the Bit2 of which the client desires a traversal of 
 *      void apply: The function that will be applied to each index
 *      bool *OK: Bool representing if the traversal succeeded
 *
 * Return: Nothing
 *
 * Expects
 *      Bit2 to be not NULL
 * Notes:
 *      Will CRE if Bit2 is NULL
 ************************/
void Bit2_map_row_major(T bit2, void apply(int x, int y, T bit2, 
                                        int elem, void *cl), bool *OK)
{
        assert(bit2 != NULL); /* CRE if Bit2 is NULL */
        int height = Bit2_height(bit2);
        int width = Bit2_width(bit2);
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        apply(j, i, bit2, Bit2_get(bit2, j, i), OK);
                }
        }
}

#undef T