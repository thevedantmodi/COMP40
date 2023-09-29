#include <stdbool.h>
#include <stdio.h>

bool process_file(char *name)
{
        FILE *f;
        f = fopen(name, "rb");
        if (f == NULL) {
                return false;
        }
        fclose(f);
        return true;
}
    
int
main(int argc, char *argv[])
{
        (void) argc;
        char *fname = argv[1];
        bool success;
        success = process_file(fname);
        if (!success) {
                printf("Could not read file %s\n", fname);
        }
}
