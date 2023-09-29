/**************************************************************
 *
 *                     seq_funcs.c
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     15 Sep 2023
 *
 *     Summary:
 *     Implementation of the functions defined in seq_funcs.h. Functions
 *     join_Seqs and insert_row_into_Seq allow the client to add the contents
 *     of a Hanson sequence or integer array to the end of a Hanson Sequence.
 *     Helper function add_int_to_Seq ensures that deep copies are made of the
 *     elements added to the Sequence.
 *
 **************************************************************/

#include "seq_funcs.h"

 /* join_Seqs
 * Takes two Hanson Sequences and copies over and appends data from the second
 * Sequence to the end of the first Sequence
 * Inputs:
        * Seq_T *first: a pointer to the Seq that will be added to
        * Seq_T *second: a pointer to the Seq whose data will be copied and
        *                appended to first
 * Returns: None
 * Expects: first and second to point to nonnull Sequences populated with
 *          int * pointers
 * Notes:
        * New space allocated on the heap for copies of the integer data to be
        * added to first
        * Will CRE if malloc fails
 */
void join_Seqs(Seq_T *first, Seq_T *second)
{
        int length = Seq_length(*second);
        for (int i = 0; i < length; i++) {
                add_int_to_Seq(first, Seq_get(*second, i));
        }
}

/* insert_row_into_Seq
 * Appends integer data from an array of integers to the end of a Hanson
 * Sequence
 * Inputs:
 * Seq_T *seq: a pointer to the Seq that will be added to
 * int **row_data: a pointer to the array of integers whose data will be
 *                 added to seq
 * int row_len: the amount of numbers stored in *row_data
 * Returns: None
 * Expects: row_data points to a nonnull array with initialized integers from
 *          indices 0 to row_len - 1
 * Notes:
 * New space allocated on the heap for copies of the integer data from
 * row_data
 * Will CRE if malloc fails
 */
void insert_row_into_Seq(Seq_T *seq, int **row_data, int row_len)
{
        for (int i = 0; i < row_len; i++) {
                add_int_to_Seq(seq, &(*row_data)[i]);
        }
}

/* add_int_to_Seq
 * Adds a new heap-allocated integer to th end of a Hanson Sequence
 * Inputs:
 * Seq_T *seq: a pointer to the Seq to add the integer to
 * int *val: a pointer to the integer whose value to copy into the Seq
 * Returns: None
 * Expects: seq and val are nonnull
 * Notes:
 * New space allocated on the heap for a copy of what val points to
 * Will CRE if malloc fails
 */
void add_int_to_Seq(Seq_T *seq, int *val)
{
        int *to_add = malloc(sizeof(*to_add));
        assert(to_add != NULL);
        *to_add = *val;
        Seq_addhi(*seq, to_add);
}