#include <stdio.h>
#include <string.h>
#include "include/bconv_format.h"

// Looks up an opcode's mnemonic + operand-flag by its enum value
static const OpEntry *find_by_op(OpCode op) {
    for (int i = 0; i < (int)BCONV_OPTABLE_LEN; i++) {
        if (BCONV_OPTABLE[i].op == op) return &BCONV_OPTABLE[i];
    }
    return 0;
}

// Writes the BL header, then each instruction as [2-char mnemonic][4-byte operand?]
int bconv_encode(FILE *out, OpCode *ops, int *operands, int count) {
    // Magic header
    fputc(BCONV_MAGIC0, out);
    fputc(BCONV_MAGIC1, out);

    for (int i = 0; i < count; i++) {
        const OpEntry *e = find_by_op(ops[i]);
        if (!e) {
            fprintf(stderr, "bconv_encode: unknown opcode %d\n", ops[i]);
            return 0;
        }
        fputc(e->mnemonic[0], out);
        fputc(e->mnemonic[1], out);

        if (e->has_operand) {
            int val = operands[i];
            fputc((val >> 24) & 0xFF, out);
            fputc((val >> 16) & 0xFF, out);
            fputc((val >> 8)  & 0xFF, out);
            fputc(val & 0xFF, out);
        }
    }
    return 1;
}