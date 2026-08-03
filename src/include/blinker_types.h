#ifndef BLINKER_TYPES_H
#define BLINKER_TYPES_H

#define MAX_SYMBOLS   256
#define MAX_CODE      65536
#define MAX_FILES     32
#define SYM_NAME_LEN  64

typedef struct {
    char name[SYM_NAME_LEN];
    int  offset;     // location within this object's code
    int  defined;    // 1 if defined here, 0 if just referenced (extern)
} Symbol;

typedef struct {
    char    filename[256];
    int     code[MAX_CODE];
    int     code_len;
    Symbol  symbols[MAX_SYMBOLS];
    int     symbol_count;
} ObjectFile;

typedef struct {
    int     code[MAX_CODE];
    int     code_len;
    Symbol  symbols[MAX_SYMBOLS];
    int     symbol_count;
} LinkedOutput;

#endif