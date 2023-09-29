#include <pnmrdr.h>
#include <stdio.h>
#include <stdlib.h>

void process_file(FILE *fp);
void read_data(unsigned height, unsigned width, Pnmrdr_T *rdr);
void print_header(Pnmrdr_mapdata *data);
void print_binary(unsigned val);
// void test_raw_pnm(FILE *fp);

int main(int argc, char *argv[]) {
    (void)argc;
    // if (argc != 3) {
    //     fprintf(stderr, "Incorrect usage");
    // }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr,
                "%s: %s %s %s\n",
                argv[0], "Could not open file ",
                argv[1], "for reading");
        exit(1);
    }

    // if (argc == 3 && argv[2] == 'r') {
    //     test_raw_pnm(fp);
    // } else {
    process_file(fp);
    // }

    // printf("%d\n%d\n%d\n%d\n", data.type, data.width, data.height, data.denominator);

    fclose(fp);
    return 0;
}

void process_file(FILE *fp) {
    Pnmrdr_T rdr = Pnmrdr_new(fp);
    Pnmrdr_mapdata data = Pnmrdr_data(rdr);

    print_header(&data);

    read_data(data.height, data.width, &rdr);

    Pnmrdr_free(&rdr);
}

void read_data(unsigned height, unsigned width, Pnmrdr_T *rdr) {
    for (int i = 0; i < (int)height; i++) {
        for (int j = 0; j < (int)width; j++) {
            unsigned val = Pnmrdr_get(*rdr);
            print_binary(val);
        }
        printf("\n");
    }
}

void print_header(Pnmrdr_mapdata *data) {
    printf("P5\n%d %d\n%d\n", (*data).width, (*data).height, (*data).denominator);
}

void print_binary(unsigned val) {
    printf("%c", val);
}

// void test_raw_pnm() {
//     Pnmrdr_T rdr = Pnmrdr_new(fp);
//     Pnmrdr_mapdata data = Pnmrdr_data(rdr);

//     print_header(&data);

//     read_data(data.height, data.width, &rdr);

//     Pnmrdr_free(&rdr);
// }
