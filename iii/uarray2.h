/**************************************************************
*
*                     uarray2.h
*
*     Assignment: iii
*     Authors:  Josh Field (jfield02) and Vedant Modi (vmodi01)
*     Date:     28 Sep 2023
*
*     Summary:
*     UArray2 is an unboxed 2D array implemented using a single Hanson UArray,
*     and that is as long as the width * height of the UArray2.
*
**************************************************************/

#ifndef __UARRAY2__H_
#define __UARRAY2__H_

#include <stdbool.h>
#include <uarray.h>

#define T UArray2_T
typedef struct T *T;

T UArray2_new(int width, int height, int size);
void UArray2_free(T *uarray2);
int UArray2_width(T uarray2);
int UArray2_height(T uarray2);
int UArray2_size (T uarray2);
void *UArray2_at(T uarray2, int x, int y);
void UArray2_map_col_major(T uarray2, void apply(int x, int y, T uarray2, 
                                        void *elem, void *cl), bool *OK);
void UArray2_map_row_major(T uarray2, void apply(int x, int y, T uarray2, 
                                        void *elem, void *cl), bool *OK);
int flattened_index(T uarray2, int x, int y);

#undef T
#endif