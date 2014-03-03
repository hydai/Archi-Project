#include "runtime.h"

instruction parseInstr(WORD_32bit instr) {
    instruction tmpInstr;
    /* get opcode first */
    tmpInstr.opcode = instr >> 26;
    // TODO: parse opcode
    return tmpInstr;
}
