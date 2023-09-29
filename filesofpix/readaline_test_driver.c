#include "readaline.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    if (argc > 1) {
        assert(argc == 2); /* c.r.e: Bad args provided */
        fp = fopen(argv[1], "r");
        assert(fp != NULL); /* c.r.e: File failed to open */
    } else {
        fp = stdin;
    }

    size_t num_bytes;
    char *line;
    num_bytes = readaline(fp, &line);
    while (num_bytes > 0) {
        for (int i = 0; i < (int)num_bytes; i++) {
            printf("%c", line[i]);
        }
        /* printf("\n"); */

        free(line);
        num_bytes = readaline(fp, &line);
        /* fprintf(stderr, "%ld\n", num_bytes); */
    }
    free(line);

    fclose(fp);

    return EXIT_SUCCESS;
}
