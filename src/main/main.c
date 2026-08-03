#include <stdio.h>
#include "include/blinker_types.h"
#include "include/blinker_object.h"
#include "include/blinker_link.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: blinker <out.bexe> <in1.bobj> [in2.bobj ...]\n");
        return 1;
    }

    ObjectFile objs[MAX_FILES];
    int obj_count = 0;

    for (int i = 2; i < argc; i++) {
        if (!object_load(argv[i], &objs[obj_count])) {
            return 1;
        }
        obj_count++;
    }

    LinkedOutput out;
    if (!link_objects(objs, obj_count, &out)) {
        return 1;
    }

    link_write(argv[1], &out);
    printf("BLinker: wrote %s (%d code words, %d symbols)\n",
           argv[1], out.code_len, out.symbol_count);
    return 0;
}