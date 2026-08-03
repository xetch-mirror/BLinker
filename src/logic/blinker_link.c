#include <stdio.h>
#include <string.h>
#include "include/blinker_link.h"

// Merges multiple ObjectFiles into one LinkedOutput,
// rebasing each object's code into the combined address space
// and resolving symbol references against defined symbols.

int link_objects(ObjectFile *objs, int obj_count, LinkedOutput *out) {
    out->code_len = 0;
    out->symbol_count = 0;

    int base_offsets[MAX_FILES];

    // Pass 1: lay out code, record base offset per file, collect defined symbols
    for (int i = 0; i < obj_count; i++) {
        base_offsets[i] = out->code_len;
        for (int j = 0; j < objs[i].code_len; j++) {
            out->code[out->code_len++] = objs[i].code[j];
        }
        for (int j = 0; j < objs[i].symbol_count; j++) {
            Symbol *s = &objs[i].symbols[j];
            if (s->defined) {
                Symbol *out_s = &out->symbols[out->symbol_count++];
                strncpy(out_s->name, s->name, SYM_NAME_LEN - 1);
                out_s->offset  = base_offsets[i] + s->offset;
                out_s->defined = 1;
            }
        }
    }

    // Pass 2: resolve extern references (defined==0) against the merged symbol table
    for (int i = 0; i < obj_count; i++) {
        for (int j = 0; j < objs[i].symbol_count; j++) {
            Symbol *s = &objs[i].symbols[j];
            if (!s->defined) {
                int found = 0;
                for (int k = 0; k < out->symbol_count; k++) {
                    if (strcmp(out->symbols[k].name, s->name) == 0) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    fprintf(stderr, "BLinker: undefined reference to '%s' (in %s)\n",
                            s->name, objs[i].filename);
                    return 0;
                }
            }
        }
    }

    return 1;
}

void link_write(const char *path, LinkedOutput *out) {
    FILE *f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "BLinker: cannot write %s\n", path);
        return;
    }
    fprintf(f, "CODE %d\n", out->code_len);
    for (int i = 0; i < out->code_len; i++) {
        fprintf(f, "%d ", out->code[i]);
    }
    fprintf(f, "\nEND\n");
    fclose(f);
}