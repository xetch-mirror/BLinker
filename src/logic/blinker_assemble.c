// src/logic/ble_assemble.c
#include <stdio.h>
#include <string.h>
#include "convert/include/bconv_format.h"

int ble_assemble(FILE *in, OpCode *ops_out, int *operands_out, int max_count) {
    char mnem[8];
    int count = 0;

    while (count < max_count && fscanf(in, "%7s", mnem) == 1) {
        int has_operand = 0;
        OpCode op = OP_UNKNOWN;

        for (int i = 0; i < (int)BCONV_OPTABLE_LEN; i++) {
            if (strcmp(BCONV_OPTABLE[i].mnemonic, mnem) == 0) {
                op = BCONV_OPTABLE[i].op;
                has_operand = BCONV_OPTABLE[i].has_operand;
                break;
            }
        }

        if (op == OP_UNKNOWN) {
            fprintf(stderr, "ble_assemble: unknown mnemonic '%s'\n", mnem);
            return -1;
        }

        int val = 0;
        if (has_operand) {
            if (fscanf(in, "%d", &val) != 1) {
                fprintf(stderr, "ble_assemble: missing operand for '%s'\n", mnem);
                return -1;
            }
        }

        ops_out[count]      = op;
        operands_out[count] = val;
        count++;
    }

    return count;
}