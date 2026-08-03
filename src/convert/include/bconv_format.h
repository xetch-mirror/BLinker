#ifndef BCONV_FORMAT_H
#define BCONV_FORMAT_H

// Magic header — first 2 bytes of every BLinker object file
#define BCONV_MAGIC0 'B'
#define BCONV_MAGIC1 'L'

// Opcode enum (internal numeric form used after decoding)
typedef enum {
    OP_NOP  = 0,
    OP_LD   = 1,
    OP_ST   = 2,
    OP_ADD  = 3,
    OP_SUB  = 4,
    OP_JMP  = 5,
    OP_RET  = 6,
    OP_CALL = 7,
    OP_HLT  = 8,
    OP_UNKNOWN = -1
} OpCode;

// Which opcodes carry a 4-byte operand after the mnemonic
// LD, ST, JMP, CALL all take an address/immediate operand.
// NOP, ADD, SUB, RET, HLT do not.

typedef struct {
    const char mnemonic[3]; // 2 chars + null terminator
    OpCode     op;
    int        has_operand;
} OpEntry;

// The actual mnemonic table — order doesn't matter, lookup is linear
static const OpEntry BCONV_OPTABLE[] = {
    { "N1", OP_NOP,  0 },
    { "K4", OP_LD,   1 },
    { "Z9", OP_ST,   1 },
    { "A4", OP_ADD,  0 },
    { "S0", OP_SUB,  0 },
    { "BB", OP_JMP,  1 },
    { "R5", OP_RET,  0 },
    { "C!", OP_CALL, 1 },
    { "R9", OP_HLT,  0 },
};

#define BCONV_OPTABLE_LEN (sizeof(BCONV_OPTABLE) / sizeof(OpEntry))

#endif