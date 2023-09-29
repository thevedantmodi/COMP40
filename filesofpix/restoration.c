/* restoration.c
 * Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 * Date created: 10 Sep 2023
 * TODO: 4 spaces > 8 spaces
 */
#include "readaline.h"
#include <assert.h>
#include <atom.h>
#include <except.h>
#include <seq.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <table.h>

/* TODO: test case with no infusion in a row, just numbers */
/* TODO: put function opening curly braces on new lines */

/*
Edit parseline to not include newline character in the infusion string
Test more edge cases (incl incomplete chars in readaline)
Extend readaline to do any length line
 */

int readfile(FILE *fp, Seq_T *original_rows);
int handle_line(char **line, size_t num_bytes, Seq_T *original_rows,
                Table_T *infusion_table, const char **original_row_infusion,
                bool *infusion_found);
int parseline(char **line, size_t num_bytes, char **infusion,
              int *infusion_len, int **row_data);
int handledata(char **infusion, int *infusion_len, int **row_data, int row_len,
               Seq_T *original_rows, Table_T *infusion_table,
               const char **original_row_infusion, bool *infusion_found);
void join_Seqs(Seq_T *first, Seq_T *second);
void insert_row_into_Seq(Seq_T *seq, int **row_data, int row_len);
void output_pgm(Seq_T *data, int height);
void free_Sequence(Seq_T *seq);
void free_Table(Table_T *table);
void free_table_entry(const void *key, void **value, void *cl);

int main(int argc, char *argv[]) {
    FILE *fp;
    if (argc > 1) {
        assert(argc == 2); /* c.r.e: Bad args provided */
        fp = fopen(argv[1], "r");
        assert(fp != NULL); /* c.r.e: File failed to open */
    } else {
        fp = stdin;
    }

    Seq_T original_rows = Seq_new(1000); /* TODO: figure out right hint */

    int height = readfile(fp, &original_rows);
    fclose(fp);

    output_pgm(&original_rows, height);

    free_Sequence(&original_rows);

    return EXIT_SUCCESS;
}

int readfile(FILE *fp, Seq_T *original_rows) {
    size_t num_bytes;
    Table_T infusion_table = Table_new(0, NULL, NULL);
    const char *original_row_infusion;
    bool infusion_found = false;
    int num_original_rows = 0;

    char *line;
    num_bytes = readaline(fp, &line);
    while (num_bytes > 0) {
        num_original_rows += handle_line(&line, num_bytes,
                                         original_rows,
                                         &infusion_table,
                                         &original_row_infusion,
                                         &infusion_found);
        free(line);
        num_bytes = readaline(fp, &line);
    }
    if (line != NULL) {
        free(line);
    }
    free_Table(&infusion_table);
    return num_original_rows;
}

int handle_line(char **line, size_t num_bytes, Seq_T *original_rows,
                Table_T *infusion_table, const char **original_row_infusion,
                bool *infusion_found) {
    char *infusion =
        malloc(sizeof(*infusion) * (num_bytes + 1));
    assert(infusion != NULL); /* c.r.e: C-string failed to alloc */
    int *row_data = malloc(sizeof(*row_data) * num_bytes);
    assert(row_data != NULL); /* c.r.e: Row data failed to alloc */

    int infusion_len;
    int row_len = parseline(line, num_bytes, &infusion, &infusion_len,
                            &row_data);

    int rows_added = handledata(&infusion, &infusion_len, &row_data, row_len,
                                original_rows, infusion_table,
                                original_row_infusion, infusion_found);

    free(infusion);
    free(row_data);

    return rows_added;
}

int parseline(char **line, size_t num_bytes, char **infusion, int *infusion_len,
              int **row_data) {
    int number_builder = -1;
    *infusion_len = 0;
    int row_data_index = 0;

    for (int i = 0; i < (int)num_bytes; i++) {
        char c = (*line)[i];
        if (c >= 48 && c <= 57) { /* character is a digit */
            if (number_builder < 0) {
                number_builder = 0;
            }
            number_builder = 10 * number_builder + (c - 48);
        } else {
            if (number_builder >= 0) {
                (*row_data)[row_data_index++] = number_builder;
                number_builder = -1;
            }
            if (c != '\n') {
                (*infusion)[(*infusion_len)++] = c;
            }
        }
    }

    if (number_builder >= 0) {
        (*row_data)[row_data_index++] = number_builder;
    }
    return row_data_index;
}

int handledata(char **infusion, int *infusion_len, int **row_data, int row_len,
               Seq_T *original_rows, Table_T *infusion_table,
               const char **original_row_infusion, bool *infusion_found) {

    const char *infusion_atom = Atom_new(*infusion, *infusion_len);

    if (*infusion_found) {
        if (*original_row_infusion == infusion_atom) {
            /* Add everything in row_data to original_rows */
            insert_row_into_Seq(original_rows, row_data, row_len);
            return 1;
        } else {
            return 0;
        }
    } else {
        Seq_T matching_line = Table_get(*infusion_table, infusion_atom);
        if (matching_line == NULL) { /* Infusion not yet in hash table */
            /* fprintf(stderr, "Entering infusion into table\n"); */
            Seq_T row = Seq_new(row_len);
            insert_row_into_Seq(&row, row_data, row_len);
            Table_put(*infusion_table, infusion_atom, row);
            return 0;
        } else { /* First duplicate infusion has been found */
            *original_row_infusion = infusion_atom;
            *infusion_found = true;

            join_Seqs(original_rows, &matching_line);
            insert_row_into_Seq(original_rows, row_data, row_len);
            return 2;
        }
    }
}

void join_Seqs(Seq_T *first, Seq_T *second) {
    int length = Seq_length(*second);
    for (int i = 0; i < length; i++) {
        int *to_add = malloc(sizeof(*to_add));
        *to_add = *(int *)Seq_get(*second, i);
        Seq_addhi(*first, to_add);
    }
}

void insert_row_into_Seq(Seq_T *seq, int **row_data, int row_len) {
    for (int i = 0; i < row_len; i++) {
        int *to_add = malloc(sizeof(*to_add));
        assert(to_add != NULL);
        *to_add = (*row_data)[i];
        Seq_addhi(*seq, to_add);
    }
}

void output_pgm(Seq_T *data, int height) {
    /* Print image header to stdout */
    int width = Seq_length(*data) / height;
    printf("P5\n%d %d\n255\n", width, height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            putchar(*((int *)Seq_get(*data, (i * width) + j)));
        }
    }
}

void free_Sequence(Seq_T *seq) {
    int len = Seq_length(*seq);
    for (int i = 0; i < len; i++) {
        free(Seq_get(*seq, i));
    }
    Seq_free(seq);
}

void free_Table(Table_T *table) {
    Table_map(*table, free_table_entry, NULL);
    Table_free(table);
}

void free_table_entry(const void *key, void **value, void *cl) {
    (void)key;
    (void)cl;
    Seq_T *seq = (Seq_T *)value;
    free_Sequence(seq);
}
