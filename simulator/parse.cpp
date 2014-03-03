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
WORD_32bit getCi(WORD_32bit instr) {
    return instr & 0xFFFF;
}
WORD_32bit getCa(WORD_32bit instr) {
    return instr & 0x2FFFFFF;
}

instruction parseOpRType(WORD_32bit instr) {
    instruction tmpInstr;
    tmpInstr.opcode = getOpcode(instr);
    return tmpInstr;
}

/*
 * Addi instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x08         | rs       | rt       | Ci       |
 * */
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

/*
 * Lw instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x23         | rs       | rt       | Ci       |
 * */
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

/*
 * Lh instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x21         | rs       | rt       | Ci       |
 * */
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

/*
 * Lhu instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x25         | rs       | rt       | Ci       |
 * */
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

/*
 * Lb instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x20         | rs       | rt       | Ci       |
 * */
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

/*
 * Lbu instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x24         | rs       | rt       | Ci       |
 * */
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

/*
 * Sw instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x2B         | rs       | rt       | Ci       |
 * */
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

/*
 * Sh instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x29         | rs       | rt       | Ci       |
 * */
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

/*
 * Sb instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x28         | rs       | rt       | Ci       |
 * */
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

/*
 * Lui instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x0F         | X        | rt       | Ci       |
 * */
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

/*
 * Andi instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x0C         | rs       | rt       | Ci       |
 * */
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

/*
 * Ori instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x0D         | rs       | rt       | Ci       |
 * */
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

/*
 * Nori instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x0E         | rs       | rt       | Ci       |
 * */
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

/*
 * Slti instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x0A         | rs       | rt       | Ci       |
 * */
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

/*
 * Beq instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x04         | rs       | rt       | Ci       |
 * */
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

/*
 * Bne instruction:
 * | 6-bit opcode | 5-bit rs | 5-bit rt | 16-bit C |
 * | 0x05         | rs       | rt       | Ci       |
 * */
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

/*
 * J instruction:
 * | 6-bit opcode | 26-bit C |
 * | 0x02         | Ca       |
 * */
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
 * | 0x03         | Ca       |
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

