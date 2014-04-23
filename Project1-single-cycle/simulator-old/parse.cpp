#include "parse.h"

/*
 * Opcode is the first 6 bits of an instruction.
 * In 32-bit instruction, we should right shift 26 bit
 * to finish the work.
 */
WORD_32bit getOpcode(WORD_32bit instr) {
    return instr >> 26;
}
WORD_32bit getRs(WORD_32bit instr) {
    return (instr >> 21) & 0x1F;
}
WORD_32bit getRt(WORD_32bit instr) {
    return (instr >> 16) & 0x1F;
}
WORD_32bit getRd(WORD_32bit instr) {
    return (instr >> 11) & 0x1F;
}
WORD_32bit getCi(WORD_32bit instr) {
    return instr & 0xFFFF;
}
WORD_32bit getCs(WORD_32bit instr) {
    return (instr >> 6) & 0x1F;
}
WORD_32bit getCa(WORD_32bit instr) {
    return instr & 0x2FFFFFF;
}
WORD_32bit getFunct(WORD_32bit instr) {
    return instr & 0x2F;
}

/*
 * R-type instruction:
 * | 6-bit  | 5-bit | 5-bit | 5-bit | 5-bit    | 6-bit |
 * | opcode | rs    | rt    | rd    | C(shamt) | funct |
 * */
instruction parseOpRType(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    // get rs
    tmpInstr.rs = getRs(instr);
    // get rt
    tmpInstr.rt = getRt(instr);
    // get rd
    tmpInstr.rd = getRd(instr);
    // get C(shamt)
    tmpInstr.cs = getCs(instr);
    // get funct
    tmpInstr.funct = getFunct(instr);
    return tmpInstr;
}

/*
 * I-type instruction:
 * | 6-bit  | 5-bit | 5-bit | 16-bit       |
 * | opcode | rs    | rt    | C(immediate) |
 * */
instruction parseIType(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    // get rs
    tmpInstr.rs = getRs(instr);
    // get rt
    tmpInstr.rt = getRt(instr);
    // get C(immediate)
    tmpInstr.ci = getCi(instr);
    return tmpInstr;
}

/*
 * J-type instruction:
 * | 6-bit opcode | 26-bit C |
 * | 0x??         | Ca       |
 * */
instruction parseJType(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    // get C(address)
    tmpInstr.ca = getCa(instr);
    return tmpInstr;
}

/*
 * Halt instruction:
 * | 6-bit opcode | 26-bit C |
 * | 0x3F         | X        |
 * */
instruction parseOpSType(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    return tmpInstr;
}

