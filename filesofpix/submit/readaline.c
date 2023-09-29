/**************************************************************
 *
 *                     readaline.c
 *
 *     Assignment: filesofpix
 *     Authors:  Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 *     Date:     10 Sep 2023
 *
 *     Summary:
 *     readaline.c contains the implementation of readaline(), as defined in
 *     readaline.h, and its dependencies. The function reads a line from a
 *     file, and saves the data from the line into a char array.
 *
 **************************************************************/
#include "readaline.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STARTLEN 1000

void expand(char **array, size_t size, size_t *capacity);

/* readaline
 * Reads a line from an open file and saves line data to a char array
 * Inputs:
 * FILE *inputfd: a pointer to the file to read from
 * char **datapp: a pointer to the char array to populate with the line
 *                data
 * Returns: size_t representing the amount of characters that have been stored
 *          in the array that datapp points to
 * Expects: input fd to point to a file that has been opened for reading
 * Notes:
 * New space allocated on the heap for char array to store line data
 * Will CRE if either parameter provided is null
 * Will CRE if malloc fails
 */
size_t readaline(FILE *inputfd, char **datapp)
{
        /* CRE if either provided parameter is NULL */
        assert(inputfd != NULL);
        assert(datapp != NULL);

        /* Allocate space on the heap for *datapp */
        size_t capacity = STARTLEN;
        *datapp = malloc(sizeof(**datapp) * capacity);
        assert(*datapp != NULL); /* c.r.e: Memory failed to allocate */

        size_t counter = 0;
        int next = getc(inputfd);
        while (next != EOF) {
                if (counter >= capacity)
                        expand(datapp, counter, &capacity);

                /* Add the char to the *datapp array */
                (*datapp)[counter++] = next;
                
                /* Exit from loop if newline found, after it is added to arr */
                if (next == '\n')
                        break;

                next = getc(inputfd);
        }

        /* Deallocate datapp if the line is empty */
        if (counter == 0) {
                free(*datapp);
                *datapp = NULL;
        }

        return counter;
}

/* expand
 * Doubles the capacity of a char array by allocating new space on the heap
 * and copying elements over
 * Inputs:
 * char **array: a pointer to the array to expand
 * size_t size: the amount of elements stored in the array
 * size_t *capacity: a pointer to the capacity of the array
 * Returns: None
 * Expects: array to point to a nonnull char array where indices 0 through
 *          size - 1 contain valid chars, capacity is nonzero
 * Notes:
 * Capacity will be updated to the new capacity (twice as large)
 * New space allocated on the heap for a larger array
 * Will CRE if malloc fails
 */
void expand(char **array, size_t size, size_t *capacity)
{
        /* Double capacity and allocate new space for the array */
        *capacity *= 2;
        char *new_array = malloc(*capacity * sizeof(*new_array));
        assert(new_array != NULL);

        /* Copy over elements into the new array */
        for (size_t i = 0; i < size; i++)
                new_array[i] = (*array)[i];

        /* free memory for old array and reassign it to new array */
        free(*array);
        *array = new_array;
}

#undef STARTLEN
