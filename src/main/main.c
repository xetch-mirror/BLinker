#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/blinker_types.h"
#include "include/blinker_objects.h"
#include "include/blinker_link.h"

int ends_with(const char *str, const char *suffix) {
    size_t len_str = strlen(str), len_suf = strlen(suffix);
    if (len_suf > len_str) return 0;
    return strcmp(str + len_str - len_suf, suffix) == 0;
}

// If the input is real assembly (.s), assemble it with nasm first.
// Returns the path to a real object file either way.
const char *assemble_if_needed(const char *path) {
    if (!ends_with(path, ".s")) return path; // already an object file

    static char obj_path[256];
    snprintf(obj_path, sizeof(obj_path), "%s.o", path);

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "nasm -f elf64 %s -o %s", path, obj_path);

    printf("BLinker: assembling %s -> %s\n", path, obj_path);
    if (system(cmd) != 0) {
        fprintf(stderr, "BLinker: nasm failed on %s\n", path);
        exit(1);
    }
    return obj_path;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: blinker <out.ble> <in1.s|in1.bobj> [in2 ...]\n");
        return 1;
    }

    ObjectFile objs[MAX_FILES];
    int obj_count = 0;

    for (int i = 2; i < argc; i++) {
        const char *obj_path = assemble_if_needed(argv[i]);
        if (!object_load(obj_path, &objs[obj_count])) {
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