/**************************************************************
 *
 *                     seq_funcs.h
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     15 Sep 2023
 *
 *     Summary:
 *     Provides functionality for Hanson Sequences used by the file_handler
 *     module to append the contents of an array or Sequence to the end of
 *     another Sequence.
 *
 **************************************************************/

#ifndef __SEQ__FUNCS__H
#define __SEQ__FUNCS__H

#include <seq.h>
#include <stdlib.h>
#include <assert.h>

void join_Seqs(Seq_T *first, Seq_T *second);
void insert_row_into_Seq(Seq_T *seq, int **row_data, int row_len);
void add_int_to_Seq(Seq_T *seq, int *val);

#endif