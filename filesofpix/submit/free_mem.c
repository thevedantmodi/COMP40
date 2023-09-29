/**************************************************************
 *
 *                     free_mem.c
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     14 Sep 2023
 *
 *     Summary:
 *     Function implementations to free memory of Hanson structures used in
 *     restoration. Will call Hanson free methods for each seq in table, and for
 *     each element malloc'ed in a seq.
 *
 **************************************************************/

#include "free_mem.h"

/* free_Sequence
 * Frees memory for a Sequence and the elements it contains
 * Inputs:
    * Seq_T *seq: a pointer to the Seq to free
 * Returns: None
 * Expects: seq is nonnull
 */
void free_Sequence(Seq_T *seq)
{
        int len = Seq_length(*seq);
        for (int i = 0; i < len; i++) {
                free(Seq_get(*seq, i));
        }
        Seq_free(seq);
}

/* free_Table
 * Frees memory for a Table and all of its entries
 * Inputs:
    * Table_T *table: a pointer to the Table to free
 * Returns: None
 * Expects: table is nonnull, the values in the Table are Hanson Sequences
 */
void free_Table(Table_T *table)
{
        Table_map(*table, free_table_entry, NULL);
        Table_free(table);
}

/* free_table_entry
 * Frees memory for Table entries where the value is a Sequence, frees the
 * Sequence itself and all of the Sequence elements
 * Inputs:
    * const void *key: a pointer to the key associated with the current entry,
    *                  not used because the keys in our usage are Atoms
    * void **value: a pointer to the value associated with the current entry,
    *               in this case a Sequence to be freed
    * void *cl: accumulator value, not used
 * Returns: None
 * Expects: value to point to a nonnull Hanson Sequence
 */
void free_table_entry(const void *key, void **value, void *cl)
{
        (void)key;
        (void)cl;
        Seq_T *seq = (Seq_T *)value;
        free_Sequence(seq);
}
