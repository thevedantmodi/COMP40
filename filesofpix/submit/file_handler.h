/**************************************************************
 *
 *                     file_handler.h
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     12 Sep 2023
 *
 *     Summary:
 *     file_handler.h contains the function definitions for file reading,
 *     extraction of infusion from the data in the corrupted file, and 
 *     retrieving the original image from the corrupted file.
 *
 **************************************************************/

#ifndef __FILE__HANDLER__H
#define __FILE__HANDLER__H

#include "readaline.h"
#include "free_mem.h"
#include "seq_funcs.h"
#include <assert.h>
#include <atom.h>
#include <seq.h>
#include <stdbool.h>
#include <table.h>
#include <stdlib.h>

int readfile(FILE *fp, Seq_T *original_rows);
int handle_line(char **line, size_t num_bytes, Seq_T *original_rows,
                Table_T *infusion_table,
                const char **original_row_infusion,
                bool *infusion_found);
int parseline(char **line, size_t num_bytes, char **infusion,
              int *infusion_len, int **row_data);
int handle_line_data(char **infusion, int *infusion_len, int **row_data,
               int row_len, Seq_T *original_rows, Table_T *infusion_table,
               const char **original_row_infusion, bool *infusion_found);

#endif