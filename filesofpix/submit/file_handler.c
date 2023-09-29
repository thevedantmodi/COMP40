/**************************************************************
 *
 *                     file_handler.c
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     12 Sep 2023
 *
 *     Summary:
 *     file_handler.c contains the function implementations for file reading,
 *     extraction of infusion from the data in the corrupted file, and 
 *     retrieving the original image from the corrupted file.
 *
 **************************************************************/

#include "file_handler.h"

/**********readfile********
 *
 * Reads the file, then extracts and stores the original data of the image
 * Inputs:
 *      FILE *fp: Filestream to read from
 *      Seq_T *original_rows: Pointer to a Hanson sequence that will be filled
 *                            with the image's original rows
 * Return: Number of rows (height of image)
 * Expects:
 *        Original rows sequence to be initialised
 *        File has already been opened
 *
 ************************/
int readfile(FILE *fp, Seq_T *original_rows)
{
        size_t num_bytes;
        Table_T infusion_table = Table_new(0, NULL, NULL);
        const char *original_row_infusion;
        bool infusion_found = false;
        int num_original_rows = 0;

        char *line;
        num_bytes = readaline(fp, &line);
        while (num_bytes > 0) {
                /* Stores data from line in appropriate data structure, and
                update count of original rows found */
                num_original_rows += handle_line(&line, num_bytes,
                                                 original_rows,
                                                 &infusion_table,
                                                 &original_row_infusion,
                                                 &infusion_found);
                free(line);
                num_bytes = readaline(fp, &line);
        }
        free_Table(&infusion_table);
        return num_original_rows;
}

/**********handleline********
 *
 * Separates a line's infusion from the image data, then decides using the
 * infusion where to store the line data.
 * Inputs:
 *      char **line: Pointer to an array containing the line as read from the
 *                   input file
 *      size_t num_bytes: The size of the input file
 *      Seq_T *original_rows: Pointer to a Seq containing the de-corrupted and
 *                            original image data
        Table_T *infusion_table: Ptr to table that stores infusions and data
                                 to compare if an infusion is original.
        const char **original_row_infusion: Points to where the original row
                                            infusion is stored once found.
 *      bool *infusion_found: A pointer to a bool representing if the original
 *                            infusion has been found.
 * Return: Number of rows in image newly found
 * Expects:
 *        Original rows sequence to be initialised
 *        File has already been opened
 * Notes:
 *      Calls helper functions parseline() and handle_line_data() to separate
 *      infusion data from image line, and validate whether the row is
 *      injected or part of the original image, respectively.
 *      CREs if memory allocation fails.
 ************************/
int handle_line(char **line, size_t num_bytes, Seq_T *original_rows,
                Table_T *infusion_table, const char **original_row_infusion,
                bool *infusion_found)
{
        char *infusion =
            malloc(sizeof(*infusion) * (num_bytes + 1));
        assert(infusion != NULL); /* c.r.e: C-string failed to alloc */
        int *row_data = malloc(sizeof(*row_data) * num_bytes);
        assert(row_data != NULL); /* c.r.e: Row data failed to alloc */

        int infusion_len;
        int row_len = parseline(line, num_bytes, &infusion, &infusion_len,
                                &row_data);

        int rows_added = handle_line_data(&infusion, &infusion_len, &row_data,
                                    row_len, original_rows, infusion_table,
                                    original_row_infusion, infusion_found);

        free(infusion);
        free(row_data);

        return rows_added;
}

/* parseline
 * Takes in a line that has been read from a corrupted image file, and
 * separates the infusion characters from the numbers in the line
 * Inputs:
        * char **line: a pointer to the array of all of the chars found in the
        *              line
        * size_t num_bytes: the amount of bytes stored in line, tells parseline
        *                   how many characters to read from line
        * char **infusion: a pointer to an array on the heap which this function
        *                  will populate with the infusion characters found in
        *                  the line
        * int *infusion_len: a pointer to an integer used to store the amount
        *                    of infusion characters found in the line
        * int **row_data: a pointer to an array of ints on the heap, which this
        *                 function will populate with the numbers found in the
        *                 line
 * Returns: The count of how many numbers were found in the line
 * Expects: infusion and row_data to be pointers to arrays that have been
 *          allocated on the heap with sufficient space to store all row data
 *          and infusion characters that the line may contain
 */
int parseline(char **line, size_t num_bytes, char **infusion, int *infusion_len,
              int **row_data)
{
        /* Create a variable that will build an int from digits found
        consecutively in the line, -1 is a signal value that it is empty */
        int number_builder = -1;
        *infusion_len = 0;
        int row_data_index = 0;

        for (int i = 0; i < (int)num_bytes; i++) {
                char c = (*line)[i];
                if (c >= 48 && c <= 57) {       /* character is a digit */
                        if (number_builder < 0) /* Signal value so set to 0 */
                                number_builder = 0;
                        int digit = c - 48;
                        number_builder = 10 * number_builder + digit;
                } else { /* character is an infusion character */
                        /* save number data if existing, then reset variable */
                        if (number_builder >= 0) {
                                (*row_data)[row_data_index++] = number_builder;
                                number_builder = -1;
                        }
                        /* Add the character to infusion arr, unless newline */
                        if (c != '\n')
                                (*infusion)[(*infusion_len)++] = c;
                }
        }
        /* Save number if it is the last thing in the row */
        if (number_builder >= 0)
                (*row_data)[row_data_index++] = number_builder;
        return row_data_index;
}

/**********handle_line_data********
 *
 * Decides whether the given image data is part of the original image, using
 * the distilled infusion sequence.
 * Inputs:
 *      char **infusion: Pointer to the infusion sequence, which has been
 *                       stripped from the line read in by the file
 *      int *infusion_len: Pointer to the length of infusion
 *      int **row_data: Pointer to array of numbers that represent image data
 *                      from the current line, sans infusion
 *      int row_len: length of the image data for this line
 *      Seq_T *original_rows: Ptr to the original rows of the image
 *      Table_T *infusion_table: Ptr to table that stores infusions and data
 *                              to compare if an infusion is original.
 *      const char **original_row_infusion: Ptr to location of Atom of
 *                                          infusion from original rows
 *                                          used to compare keys in table
 *      bool *infusion_found: Ptr to bool reflecting if the image
                              has already been found
 * Return: Number of newly added rows
 * Expects:
 *        Original rows sequence, table to be initialised
 *        Infusion and row data are already extracted and init'ed
 * Notes:
 *      Infusion and row length are provided so the program knows when to
 *      terminate reading each one.
 *      Successfully found image rows are stored in *original_rows
 *
 *
 ************************/
int handle_line_data(char **infusion, int *infusion_len, int **row_data,
                     int row_len, Seq_T *original_rows, Table_T *infusion_table,
                     const char **original_row_infusion, bool *infusion_found)
{
        const char *infusion_atom = Atom_new(*infusion, *infusion_len);

        if (*infusion_found) {
                /* Check if the current infusion matches the correct infusion
                sequence for the image */
                if (*original_row_infusion == infusion_atom) {
                        /* Add everything in row_data to original_rows */
                        insert_row_into_Seq(original_rows, row_data, row_len);
                        return 1; /* 1 original row added to Seq */
                } else {
                        return 0; /* 0 original rows added to Seq */
                }
        } else {
                Seq_T matching_line = Table_get(*infusion_table, infusion_atom);
                if (matching_line == NULL) {
                        /* Infusion not yet in hash table, make new entry */
                        Seq_T row = Seq_new(row_len);
                        insert_row_into_Seq(&row, row_data, row_len);
                        Table_put(*infusion_table, infusion_atom, row);
                        return 0; /* 0 original rows added to Seq */
                } else {
                        /* Duplicate infusion found, curr infusion is correct */
                        *original_row_infusion = infusion_atom;
                        *infusion_found = true;

                        /* Add in the matching row from the hash table and the
                        current row into the original rows Seq */
                        join_Seqs(original_rows, &matching_line);
                        insert_row_into_Seq(original_rows, row_data, row_len);
                        return 2; /* 2 original rows added to Seq */
                }
        }
}