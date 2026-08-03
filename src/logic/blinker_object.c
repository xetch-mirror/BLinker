#include <stdio.h>
#include <string.h>
#include "blinker_objects.h"

// Expected on-disk format (plain text, one token per line):
//   CODE <count>
//   <int> <int> ... (count of them)
//   SYM <name> <offset> <defined 0|1>
//   ... repeated ...
//   END

int object_load(const char *path, ObjectFile *obj) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "BLinker: cannot open %s\n", path);
        return 0;
    }

    strncpy(obj->filename, path, sizeof(obj->filename) - 1);
    obj->code_len = 0;
    obj->symbol_count = 0;

    char tag[16];
    while (fscanf(f, "%15s", tag) == 1) {
        if (strcmp(tag, "CODE") == 0) {
            int count;
            fscanf(f, "%d", &count);
            for (int i = 0; i < count; i++) {
                fscanf(f, "%d", &obj->code[obj->code_len++]);
            }
        } else if (strcmp(tag, "SYM") == 0) {
            Symbol *s = &obj->symbols[obj->symbol_count++];
            fscanf(f, "%63s %d %d", s->name, &s->offset, &s->defined);
        } else if (strcmp(tag, "END") == 0) {
            break;
        }
    }

    fclose(f);
    return 1;
}

void object_dump(ObjectFile *obj) {
    printf("Object: %s\n", obj->filename);
    printf("  code_len=%d\n", obj->code_len);
    for (int i = 0; i < obj->symbol_count; i++) {
        Symbol *s = &obj->symbols[i];
        printf("  sym %-20s offset=%-5d defined=%d\n", s->name, s->offset, s->defined);
    }
}