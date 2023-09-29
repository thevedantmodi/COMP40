/* readaline.c
 * Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 * Date created: 10 Sep 2023
 */
#include "readaline.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 1000

/* TODO: Think about potential invalid input files like empty file or
incomplete chars (less than 8 bits), read documentation of getc */

size_t readaline(FILE *inputfd, char **datapp) {
    assert(inputfd != NULL);
    assert(datapp != NULL);

    *datapp = malloc(sizeof(**datapp) * MAXLEN);
    assert(*datapp != NULL); /* c.r.e: Memory failed to allocate */

    size_t counter = 0;
    char next = getc(inputfd); /* TODO: Error while reading file */
    while (next != EOF) {
        /* Change here to change size restriction on file length */
        if (counter >= MAXLEN) {
            fprintf(stderr, "readaline: input line too long\n");
            exit(4);
        }

        (*datapp)[counter++] = next;
        if (next == '\n')
            break;

        next = getc(inputfd);
    }

    if (counter == 0) {
        free(*datapp);
        *datapp = NULL;
    }

    return counter;
}

#undef MAXLEN
