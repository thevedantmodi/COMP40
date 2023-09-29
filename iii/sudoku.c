/**************************************************************
*
*                     sudoku.c
*
*     Assignment: iii
*     Authors:  Josh Field (jfield02) and Vedant Modi (vmodi01)
*     Date:     25 September 2023
*
*     Summary:
*     sudoku takes a sudoku solution and determines if the solution is
*     a correct solution to the puzzle (either through stdin or file
*     input). The program is a predicate, and will succeed if the
*     solution is valid, and fail otherwise.
*
**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "uarray2.h"
#include <pnmrdr.h>
#include <assert.h>
#include <set.h>
#include <seq.h>

#define SUBMAP_IT 3
#define MAX_VAL 9

UArray2_T store_solution(FILE *fp, bool *correct_format);
bool validate_col(UArray2_T image);
bool validate_row(UArray2_T image);
bool validate_boxes(UArray2_T image);
void check_repeat_col(int x, int y, UArray2_T image, void *elem, void *OK);
void check_repeat_row(int x, int y, UArray2_T image, void *elem, void *OK);
bool check_submap(int x, int y, UArray2_T image);
bool check_submap_duplicates(int col, int row, UArray2_T image, 
                                Seq_T search_space);


/********** main ********
 *
 * Determines if a given sudoku solution is valid or errenous.
 *
 * Parameters:
 *      int argc: Number of arguments
 *      char *argv[]: Array of arguments
 * Return: EXIT_SUCCESS if the provided sudoku solution is valid, EXIT_FAILURE 
 *         otherwise
 *
 * Expects
 *      argc to be 1 or 2
 *      File to be a valid file, or input via stdin to be given
 * Notes:
 *      Will CRE if a provided file fails to open
 ************************/
int main(int argc, char *argv[])
{
        assert(argc == 1 || argc == 2);
        FILE *fp;
        if (argc > 1) {
                fp = fopen(argv[1], "r");
                assert(fp != NULL); /* CRE: File failed to open */
        } else {
                fp = stdin;
        }

        bool correct_format = true;
        UArray2_T image = store_solution(fp, &correct_format);

        bool valid = correct_format
                     && validate_col(image)
                     && validate_row(image)
                     && validate_boxes(image);

        UArray2_free(&image);

        fclose(fp);    
        return (valid) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/********** store_solution ********
 *
 * Reads sudoku solution from file and ensures dimensions, maxval are correct
 *
 * Parameters:
 *      FILE *fp: A file stream containing a sudoku solution
 *      bool *correct_format: A boolean variable representing if the format of 
 *                            the provide file matches that of a sudoku 
 *                            solution (9x9, maxval of 9).
 * Return: A UArray2_T representing the sudoku solution provided
 *
 * Expects
 *      FILE *fp has been opened successfully
 *      bool *correct_format has been initialised
 ************************/
UArray2_T store_solution(FILE *fp, bool *correct_format)
{
        Pnmrdr_T rdr = Pnmrdr_new(fp);
        Pnmrdr_mapdata data = Pnmrdr_data(rdr);

        *correct_format &= data.width == MAX_VAL 
                           && data.height == MAX_VAL 
                           && data.denominator == MAX_VAL;

        UArray2_T image = UArray2_new(data.width, data.height, sizeof(int));

        for (int i = 0; i < (int)data.height; i++) {
                for (int j = 0; j < (int)data.width; j++) {
                        unsigned val = Pnmrdr_get(rdr);
                        *((int *)UArray2_at(image, j, i)) = (int)val;
                }
        }

        Pnmrdr_free(&rdr);

        return image;
}

/********** validate_col ********
 *
 * validates the column invariant for the sudoku solution (each column must not 
 * have repetition in value, no value may be zero)
 *
 * Parameters:
 *      UArray2_T image: A UArray2 representation of the sudoku solution
 * Return: true if the column invariant stands, false if not
 *
 * Expects
 *      image may not be NULL
 *      check_repeat_col is not NULL
 * Notes:
 *      Will CRE if image is NULL
 ************************/
bool validate_col(UArray2_T image)
{
        assert(image != NULL); /* CRE if image is NULL */
        bool OK = true;

        /* CRE if check_repeat_col is NULL */
        assert(check_repeat_col != NULL);
        UArray2_map_col_major(image, check_repeat_col, &OK);

        return OK;
}

/********** validate_row ********
 *
 * validates the row invariant for the sudoku solution (each row must not 
 * have repetition in value, no value may be zero)
 *
 * Parameters:
 *      UArray2_T image: A UArray2 representation of the sudoku solution
 * Return: true if the row invariant stands, false if not
 *
 * Expects
 *      image may not be NULL
 *      check_repeat_row is not NULL
 * Notes:
 *      Will CRE if image is NULL
 ************************/
bool validate_row(UArray2_T image)
{
        assert(image != NULL); /* CRE if image is NULL */
        bool OK = true;

        /* CRE if check_repeat_row is NULL */
        assert(check_repeat_row != NULL);
        UArray2_map_row_major(image, check_repeat_row, &OK);

        return OK;
}

/********** validate_boxes ********
 *
 * validates the box invariant for the sudoku solution (each 3x3 submap must 
 * not have repetition in value, no value may be zero)
 *
 * Parameters:
 *      UArray2_T image: A UArray2 representation of the sudoku solution
 * Return: true if the box invariant stands, false if not
 *
 * Expects
 *      image may not be NULL
 * Notes:
 *      Will CRE if image is NULL
 ************************/
bool validate_boxes(UArray2_T image)
{
        /* Check the 3x3 grid */
        assert(image != NULL); /* CRE if image is NULL */
        for (int i = 0; i < UArray2_width(image); i += SUBMAP_IT) {
                for (int j = 0; j < UArray2_height(image); j+= SUBMAP_IT) {
                        /* Checks at the top left square of each
                           submap in the sudoku grid */
                        if (!check_submap(i, j, image)) {
                                return false;
                        }
                }
        }
        
        return true;
}

/********** check_repeat_col ********
 *
 * Checks if a given digit in a map has repeats in its column.
 *
 * Parameters:
 *      int x: the index of the selected element's column
 *      int y: the index of the selected element's row
 *      UArray2_T image: A UArray2 representation of the sudoku solution
 *      void *elem: a pointer directing to the element in the map
 *      void *OK: A pointer directing to a  is the solution is valid
 *      
 * Return: none
 *
 * Expects
 *      image may not be NULL
 *      OK cannot be NULL (*OK is initialized)
 *      elem cannot be NULL (*elem is initialized)
 *      
 * Notes:
 *      Will CRE if image is NULL
 *      Will CRE if OK is NULL
 *      Will CRE if elem is NULL
 ************************/
void check_repeat_col(int x, int y, UArray2_T image, void *elem, void *OK) 
{
        /* CRE if image, elem, or OK is NULL */
        assert(image != NULL && elem != NULL && OK != NULL); 
        bool *valid = OK;
        int *elem_p = elem;
        if (*elem_p <= 0 || *elem_p > MAX_VAL) { /* Intensity has to be [1,9]*/
                /* Once an invariant is broken, stop searching */
                *valid = false;  
                return;
        }

        /* Check for duplicates in the same column, and lesser (already seen)
        row than the selected digit in the map */
        for (int i = y - 1; i >= 0; i--) {
                if (*elem_p == *((int *)UArray2_at(image, x, i))){
                        *valid = false;
                        return;
                }
        }
}

/********** check_repeat_row ********
 *
 * Checks if a given digit in a map has repeats in its row.
 *
 * Parameters:
 *      int x: the index of the selected element's column
 *      int y: the index of the selected element's row
 *      UArray2_T image: A UArray2 representation of the sudoku solution
 *      void *elem: a pointer directing to the element in the map
 *      void *OK: A pointer directing to a  is the solution is valid
 *      
 * Return: none
 *
 * Expects
 *      image may not be NULL
 *      OK cannot be NULL (*OK is initialized)
 *      elem cannot be NULL (*elem is initialized)
 *      
 * Notes:
 *      Will CRE if image is NULL
 *      Will CRE if OK is NULL
 *      Will CRE if elem is NULL
 ************************/
void check_repeat_row(int x, int y, UArray2_T image, void *elem, void *OK) 
{
        assert(image != NULL); /* CRE if image is NULL */
        bool *valid = OK;
        int *elem_p = elem;
        if (*elem_p <= 0 || *elem_p > 9) { /* Intensity has to be [1,9] */
                *valid = false;
                return;
        }
        
        /* Check for duplicates in the same row, and lesser (already seen)
        column than the selected digit in the map */
        for (int i = x - 1; i >= 0; i--) {
                if (*elem_p == *((int *)UArray2_at(image, i, y))){
                        *valid = false;
                        return;
                }
        }
}

/********** check_submap ********
*
* For a given submap of a sudoku puzzle, checks if the box invariant holds
*
* Parameters: 
*        int x: Col index of the left and topmost pixel in the submap
*        int y: Row index of the left and topmost pixel in the submap
*        UArray2_T image: Image representing the full sudoku image
*
* Return: returns if the submap is valid
*
* Expects:
*       image is not NULL and created sequence allocates
* Notes:
*      Will CRE if image is null or sequence fails to alloc
*
************************/
bool check_submap(int x, int y, UArray2_T image) {
        Seq_T search_space = Seq_new(8);
        assert(search_space != NULL); /* CRE if sequence is null */
        
        /* Passes in the left and topmost pixel, and then searches the 
        * 3x3 submap to the right and below of the pixel in the helper */
        for (int i = 0; i < SUBMAP_IT; i++) {
                for (int j = 0; j < SUBMAP_IT; j++) {
                        if (!check_submap_duplicates(x + i, y + j,
                                                     image, search_space)) {
                                /* Stop searching if broken invariant,
                                * but don't forget to free before. */
                                Seq_free(&search_space);
                                return false;
                        }
                }
        }
        Seq_free(&search_space);
        return true;
}

/********** check_submap_duplicates ********
 *
 * Compares one element to all previous elements in the 3x3 submap to ensure no 
 * duplicates are found
 *
 * Parameters:
 *      int col: an integer representing the left most index of the 3x3 submap
 *      int row: an integer representing the top most index of the 3x3 submap
 *      UArray2_T image: A UArray2 representation of the sudoku solution
 *      Seq_T search_space: A queue containing all previously searched elements 
 *                          in the 3x3 submap
 * Return: true if the box invariant stands, false if not
 *
 * Expects
 *      image may not be NULL
 *      search_space may not be NULL
 * Notes:
 *      Will CRE if image is NULL
 *      Will CRE if search_space is NULL
 ************************/
bool check_submap_duplicates(int col, int row, UArray2_T image, 
                                Seq_T search_space)
{
        /* CRE if search_space or image is null */
        assert(search_space != NULL && image != NULL); 
        int search_times = Seq_length(search_space);
        for (int i = 0; i < search_times; i++) {
                int *seen = Seq_remlo(search_space);
                int *digit = UArray2_at(image, col, row);
                if (*seen == *digit) {
                        return false;
                }
                Seq_addhi(search_space, seen);
        }
        Seq_addhi(search_space, UArray2_at(image, col, row));
        return true;
}


#undef SUBMAP_IT
#undef MAX_VAL