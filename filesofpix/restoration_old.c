/* restoration.c
 * Sarah Grand (sgrand01) and Vedant Modi (vmodi01)
 * Date created: 10 Sep 2023
 * TODO: 4 spaces > 8 spaces
 */
#include "readaline.h"
#include <assert.h>
#include <atom.h>
#include <except.h>
#include <list.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <table.h>

/* TODO: test case with no infusion in a row, just numbers */
/* TODO: put function opening curly braces on new lines */

void readfile(FILE *fp, List_T *original_rows);
void parseline(char **line, size_t num_bytes, char **infusion, int **row_data);
void handle_line(char **line, size_t num_bytes);
void handledata(char **infusion, int **row_data, int row_len,
                List_T *original_rows, Table_T *infusion_table,
                const char **original_row_infusion, bool *infusion_found);
void output_pgm(List_T *rows);
void join_Seqs(Seq_T *first, Seq_T *second);
void insert_row_into_Seq(Seq_T *seq, int **row_data);

int main(int argc, char *argv[]) {
    FILE *fp;
    if (argc > 1) {
        assert(argc == 2); /* c.r.e: Bad args provided */
        fp = fopen(argv[1], "r");
        assert(fp != NULL); /* c.r.e: File failed to open */
    } else {
        fp = stdin;
    }

    List_T original_rows = NULL;

    readfile(fp, &original_rows);
    fclose(fp);

    output_pgm(&original_rows);

    List_free(&original_rows);

    return EXIT_SUCCESS;
}

void readfile(FILE *fp, List_T *original_rows) {
    size_t num_bytes;
    Table_T infusion_table = Table_new(0, NULL, NULL);
    const char *original_row_infusion;
    bool infusion_found = false;

    char *line;
    num_bytes = readaline(fp, &line);
    while (num_bytes > 0) {
        char *infusion =
            malloc(sizeof(*infusion) * (num_bytes + 1));
        assert(infusion != NULL); /* c.r.e: C-string failed to alloc */
        int *row_data = malloc(sizeof(*row_data) * num_bytes);
        assert(row_data != NULL); /* c.r.e: Row data failed to alloc */
        fprintf(stderr, "Allocated row_data at address %p\n", (void *)row_data);

        int row_len = parseline(&line, num_bytes, &infusion, &row_data);

        handledata(&infusion, &row_data, row_len, original_rows,
                   &infusion_table, &original_row_infusion, &infusion_found);

        free(infusion);
        /* free(row_data); */

        free(line);
        num_bytes = readaline(fp, &line);
        /* fprintf(stderr, "%ld\n", num_bytes); */
    }
    if (line != NULL) {
        free(line);
    }
    Table_free(&infusion_table);
}

int parseline(char **line, size_t num_bytes, char **infusion, int **row_data) {
    int number_builder = -1;
    int infusion_index = 0;
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
                /*  fprintf(stderr, "Adding number %d\n", number_builder); */
                number_builder = -1;
            }
            if (c != '\n') {
                (*infusion)[infusion_index++] = c;
            }
        }
    }

    if (number_builder >= 0) {
        (*row_data)[row_data_index++] = number_builder;
        /* fprintf(stderr, "Adding number %d\n", number_builder); */
    }
    (*infusion)[infusion_index] = '\0';
    return row_data_index;
}

void handledata(char **infusion, int **row_data, int row_len,
                List_T *original_rows, Table_T *infusion_table,
                const char **original_row_infusion, bool *infusion_found) {

    fprintf(stderr, "Row data:\n");
    for (int i = 0; i < row_len; i++) {
        fprintf(stderr, "%d ", (*row_data)[i]);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "row_len: %d\n", row_len);

    const char *infusion_atom = Atom_string(*infusion);

    if (*infusion_found) {
        /* Line matching_line = Table_get(*infusion_table, *original_row_infusion);
        fprintf(stderr, "Matching line: %d %d\n", **(matching_line->row_data), *(*(matching_line->row_data) + 1)); */

        if (*original_row_infusion == infusion_atom) {
            /* Next 2 lines delete, just for testing */

            Line line = create_line(row_data, row_len);
            fprintf(stderr, "Just passed row_data from address %p\n", (void *)*row_data);

            fprintf(stderr, "%d %d\n", **(line->row_data), *(*(line->row_data) + 1));
            *original_rows = List_push(*original_rows, line);
        }
    } else {
        Line line = create_line(row_data, row_len);
        fprintf(stderr, "current line created at address %p\n", (void *)line);
        fprintf(stderr, "%d %d\n", **(line->row_data), *(*(line->row_data) + 1));
        Line matching_line = Table_get(*infusion_table, infusion_atom);
        fprintf(stderr, "matching line is at address %p\n", (void *)matching_line);
        fprintf(stderr, "Matching line is %s\n",
                (matching_line == NULL ? "Null" : "Not null"));

        if (matching_line == NULL) {
            fprintf(stderr, "Entering infusion into table\n");
            Table_put(*infusion_table, infusion_atom, line);
        } else {
            fprintf(stderr, "Matching line: %d %d\n",
                    **(matching_line->row_data), *(*(matching_line->row_data) + 1));
            *original_row_infusion = infusion_atom;
            *infusion_found = true;
            fprintf(stderr,
                    "The infusion for original rows is %s\n", *original_row_infusion);
            *original_rows = List_push(*original_rows, matching_line);
            *original_rows = List_push(*original_rows, line);
        }
    }
    fprintf(stderr, "Infusion found: %s\n\n\n", (*infusion_found ? "True" : "False"));
}

Line create_line(int **row_data, int row_len) {
    Line line = malloc(sizeof(*line));
    line->row_data = row_data;
    fprintf(stderr, "created line from row_data at address %p\n", (void *)(*row_data));
    line->row_length = row_len;
    return line;
}

void output_pgm(List_T *rows) {
    /* TODO: Change here P2 > P5 to implement raw pgm in place of plain pgm */
    /* Print image header to stdout */
    int height = List_length(*rows);
    int width = 0;
    *rows = List_reverse(*rows);
    void *front;
    *rows = List_pop(*rows, &front);
    Line front_line = (Line)front;
    if (front_line != NULL) {
        width = front_line->row_length;
    }
    /* TODO: Why can't we just use a Line to pass a void** */
    /* TODO: Support no crashing on empty list (if List is null)*/
    printf("P2\n%d %d\n255\n", width, height);

    /* printf("%d", (**(front_line->row_data))); */
    /* while (front_line != NULL) { */
    /* int max_index = front_line->row_length - 1;
    for (int i = 0; i < max_index; i++) {
        printf("%d ", (*(front_line->row_data))[i]);
    }
    printf("%d\n", (*(front_line->row_data))[max_index]); */

    /* *rows = List_pop(*rows, &front);
    front_line = (Line)front; */
    /* } */

    /* make sure to recycle memory for Line structs as they are popped from list */
    /* make sure to free data that is not put in line or hash table */
}
