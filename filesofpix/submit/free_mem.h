/**************************************************************
 *
 *                     free_mem.h
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     14 Sep 2023
 *
 *     Summary:
 *     Function definitions to free memory of Hanson structures used in
 *     restoration
 *
 **************************************************************/

#ifndef __FREE_MEM__H
#define __FREE_MEM__H

#include <assert.h>
#include <seq.h>
#include <stdlib.h>
#include <table.h>

void free_Sequence(Seq_T *seq);
void free_Table(Table_T *table);
void free_table_entry(const void *key, void **value, void *cl);

#endif
