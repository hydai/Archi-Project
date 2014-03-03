#include "parse.h"

/*
 * Opcode is the first 6 bits of an instruction.
 * In 32-bit instruction, we should right shift 26 bit
 * to finish the work.
 */
WORD_32bit getOpcode(WORD_32bit instr) {
    return instr >> 26;
}

instruction parseOpFunct(WORD_32bit instr) {
    instruction tmpInstr;
    tmpInstr.opcode = getOpcode(instr);

    return tmpInstr;
}
instruction parseOpAddi(WORD_32bit instr) {
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
instruction parseOpLw(WORD_32bit instr) {
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
instruction parseOpLh(WORD_32bit instr) {
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
instruction parseOpLhu(WORD_32bit instr) {
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
instruction parseOpLb(WORD_32bit instr) {
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
instruction parseOpLbu(WORD_32bit instr) {
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
instruction parseOpSw(WORD_32bit instr) {
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
instruction parseOpSh(WORD_32bit instr) {
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
instruction parseOpSb(WORD_32bit instr) {
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
instruction parseOpLui(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    // get rt
    tmpInstr.rt = getRt(instr);
    // get C(immediate)
    tmpInstr.ci = getCi(instr);
    return tmpInstr;
}
instruction parseOpAndi(WORD_32bit instr) {
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
instruction parseOpOri(WORD_32bit instr) {
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
instruction parseOpNori(WORD_32bit instr) {
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
instruction parseOpSlti(WORD_32bit instr) {
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
instruction parseOpBeq(WORD_32bit instr) {
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
instruction parseOpBne(WORD_32bit instr) {
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
instruction parseOpJ(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    // get C(address)
    tmpInstr.ca = getCa(instr);
    return tmpInstr;
}
/*
 * Jal instruction:
 * | 6-bit opcode | 26-bit C |
 * | 0x3F         | Ca       |
 * */
instruction parseOpJal(WORD_32bit instr) {
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
instruction parseOpHalt(WORD_32bit instr) {
    instruction tmpInstr;
    // get opcode
    tmpInstr.opcode = getOpcode(instr);
    return tmpInstr;
}

