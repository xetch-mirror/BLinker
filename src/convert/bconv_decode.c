#include <stdio.h>
#include <string.h>
#include "bconv_format.h"

static OpCode find_by_mnemonic(char c0, char c1, int *has_operand) {
    for (int i = 0; i < (int)BCONV_OPTABLE_LEN; i++) {
        if (BCONV_OPTABLE[i].mnemonic[0] == c0 &&
            BCONV_OPTABLE[i].mnemonic[1] == c1) {
            *has_operand = BCONV_OPTABLE[i].has_operand;
            return BCONV_OPTABLE[i].op;
        }
    }
    *has_operand = 0;
    return OP_UNKNOWN;
}

// Reads the BL header, then decodes instructions one at a time.
// Returns number of instructions decoded, or -1 on a bad/missing magic header.
int bconv_decode(FILE *in, OpCode *ops_out, int *operands_out, int max_count) {
    int m0 = fgetc(in);
    int m1 = fgetc(in);
    if (m0 != BCONV_MAGIC0 || m1 != BCONV_MAGIC1) {
        fprintf(stderr, "bconv_decode: bad magic header (not a BLinker object)\n");
        return -1;
    }

    int count = 0;
    int c0, c1;
    while (count < max_count && (c0 = fgetc(in)) != EOF) {
        c1 = fgetc(in);
        if (c1 == EOF) break;

        int has_operand;
        OpCode op = find_by_mnemonic((char)c0, (char)c1, &has_operand);
        if (op == OP_UNKNOWN) {
            fprintf(stderr, "bconv_decode: unknown mnemonic '%c%c'\n", c0, c1);
            return -1;
        }

        int val = 0;
        if (has_operand) {
            int b0 = fgetc(in), b1 = fgetc(in), b2 = fgetc(in), b3 = fgetc(in);
            if (b3 == EOF) { fprintf(stderr, "bconv_decode: truncated operand\n"); return -1; }
            val = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
        }

        ops_out[count] = op;
        operands_out[count] = val;
        count++;
    }

    return count;
}