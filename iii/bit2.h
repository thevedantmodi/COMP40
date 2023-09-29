/**************************************************************
*
*                     bit2.h
*
*     Assignment: iii
*     Authors:  Josh Field (jfield02) and Vedant Modi (vmodi01)
*     Date:     23 Sep 2023
*
*     Summary:
*     Bit2 is an implementation of a 2D bit vector, implemented by using a 1D bit 
*     vector with converted indices.
*
**************************************************************/

#include <stdbool.h>
#include <bit.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mem.h>

#ifndef __BIT2__H_
#define __BIT2__H_
#define T Bit2_T

typedef struct T *T;


T Bit2_new(int width, int height);
void Bit2_free(T *bit2);
int Bit2_get(T bit2, int x, int y);
int Bit2_put(T bit2, int x, int y, int bit);
int Bit2_width(T bit2);
int Bit2_height(T bit2);
void Bit2_map_col_major(T bit2, void apply(int x, int y, T bit2, 
                                        int elem, void *cl), bool *OK);
void Bit2_map_row_major(T bit2, void apply(int x, int y, T bit2, 
                                        int elem, void *cl), bool *OK);
int flattened_index(T bit2, int x, int y);

#undef T
#endif