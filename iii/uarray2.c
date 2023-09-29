/**************************************************************
*
*                     uarray2.c
*
*     Assignment: iii
*     Authors:  Josh Field (jfield02) and Vedant Modi (vmodi01)
*     Date:     24 Sep 2023
*
*     Summary:
*     Implementation file for UArray2, as defined in uarray2.h. Implemented
*     by using a Hanson UArray, that stores each row of the UArray2 
*     continuously.
*
**************************************************************/

#include "uarray2.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mem.h>

#define T UArray2_T

struct T {
        UArray_T array;
        int vertical;
        int horizontal;
};

/********** flattened_index ********
 *
 * Returns the corresponding 1 dimensional index for the provided two
 * dimensional index
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires to access
 *      int x: The distance from the top of the array
 *      int y: The distance from the left of the array
 *
 * Return: The 1 dimesional index for the given two dimensional index
 *
 * Expects
 *      UArray2 to be not NULL
 *      x = [0, width) and x is an integer
 *      y = [0, height) and y is an integer
 * Notes:
 *      Will CRE if UArray2 is NULL 
 *      Will CRE if the range expectations are violated
 ************************/
int flattened_index(T uarray2, int x, int y) 
{
        assert(uarray2 != NULL); /* CRE if UArray2 is NULL */
        int height = UArray2_height(uarray2);
        int width = UArray2_width(uarray2);

        /* CRE for out of range element */
        assert(x < width && x >= 0);
        assert(y < height && y >= 0); 
        
        return (width * y) + x;
} 

/********** UArray2_new ********
 *
 * Initializes a new UArray2 struct and returns a pointer to the new struct
 *
 * Parameters:
 *      int width: the number of columns in the array
 *      int height: the number of rows in the array
 *      int size: the number of bits each element take up
 *
 * Return: A pointer to the new UArray2 struct, with init'ed dimensions
 *
 * Expects
 *      all parameters to be positive integers
 * Notes:
 *      Will CRE if the integers do not match expectations
 ************************/
T UArray2_new(int width, int height, int size)
{
        T Uarray2_p; 
        NEW0(Uarray2_p);
        
        assert(width >= 0 && height >= 0); /* CRE if negative dimensions */
        
        Uarray2_p->horizontal = width;
        Uarray2_p->vertical = height;

        /* Rows of the UArray2 stored contiguously in UArray -> 
        *  length of the UArray is the number of items in each row (width)
        *  times the number of rows (height)
        */
        int length = width * height; 

        assert(size > 0);

        Uarray2_p->array = UArray_new(length, size);
        assert(Uarray2_p->array != NULL);

        return Uarray2_p;
}

/********** UArray2_free ********
 *
 * Clears the UArray2 from memory
 *
 * Parameters:
 *      T *uarray2: address of the UArray2 that the client wishes to free
 *
 * Return: Nothing
 *
 * Expects
 *      Memory to not be freed, UArray2_ will not be null
 * Notes:
 *      Will CRE if free has been called on already free memory
 ************************/
void UArray2_free(T *uarray2)
{        
        /* CRE if UArray2 or *UArray2 is NULL */
        assert(uarray2 != NULL && *uarray2 != NULL); 
        UArray_free(&((*uarray2)->array));
        FREE(*uarray2);
}

/********** UArray2_width ********
 *
 * Returns the width of the provided UArray2
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires the width
 *
 * Return: Number of columns in the array
 *
 * Expects
 *      UArray2 to not be NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
int UArray2_width(T uarray2)
{
        assert(uarray2 != NULL); /* CRE if UArray2 is NULL */
        return uarray2->horizontal;
}

/********** UArray2_height ********
 *
 * Returns the height of the provided UArray2
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires the width
 *
 * Return: Number of rows in the array
 *
 * Expects
 *      UArray2 to be not NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
int UArray2_height(T uarray2)
{
        assert(uarray2 != NULL); /* CRE if UArray2 is NULL */
        return uarray2->vertical;
}

/********** UArray2_size ********
 *
 * Returns the size of the provided UArray2
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires the size
 *
 * Return: Size of each element in the array in bits
 *
 * Expects
 *      UArray2 to be not NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
int UArray2_size (T uarray2)
{
        assert(uarray2 != NULL); /* CRE if UArray2 is NULL */
        return UArray_size(uarray2->array);
}

/********** UArray2_at********
 *
 * Returns the height of the provided UArray2
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires the height
 *      int x: The distance from the top of the array
 *      int y: The distance from the left of the array
 *
 * Return: Number of rows in the array
 *
 * Expects
 *      UArray2 to be not NULL
 *      x = [0, width) and x is an integer
 *      y = [0, height) and y is an integer
 * Notes:
 *      Will CRE if UArray2 is NULL 
 *      Will CRE if the range expectations are violated
 ************************/
void *UArray2_at(T uarray2, int x, int y) 
{
        assert(uarray2 != NULL); /* CRE if UArray2 is NULL */
        return UArray_at(uarray2->array, flattened_index(uarray2, x, y));
}

/********** UArray2_map_col_major ********
 *
 * Traverses the array where columns increase faster than rows, applying 
 * the provided function to each intialized index.
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires a traversal of 
 *      void apply: The function that will be applied to each index
 *      bool *OK: Bool representing if the traversal succeeded
 *
 * Return: Nothing
 *
 * Expects
 *      UArray2 to be not NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
void UArray2_map_col_major(T uarray2, void apply(int x, int y, T uarray2, 
                                        void *elem, void *cl), bool *OK) 
{
        int width = UArray2_width(uarray2);
        int height = UArray2_height(uarray2);
        for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                        apply(i, j, uarray2, UArray2_at(uarray2, i, j), OK);
                }
        }
}

/********** UArray2_map_row_major ********
 *
 * Traverses the array where rows increase faster than columns, applying 
 * the provided function to each intialized index.
 *
 * Parameters:
 *      T uarray2: the UArray2 of which the client desires a traversal of 
 *      void apply: The function that will be applied to each index
 *      bool *OK: Bool representing if the traversal succeeded
 *
 * Return: Nothing
 *
 * Expects
 *      UArray2 to be not NULL
 * Notes:
 *      Will CRE if UArray2 is NULL
 ************************/
void UArray2_map_row_major(T uarray2, void apply(int x, int y, T uarray2, 
                                        void *elem, void *cl), bool *OK) 
{
        int width = UArray2_width(uarray2);
        int height = UArray2_height(uarray2);
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        apply(j, i, uarray2, UArray2_at(uarray2, j, i), OK);
                }
        }
}      


#undef T