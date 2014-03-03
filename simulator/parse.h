#ifndef PARSE_H
#define PARSE_H

#include "const.h"
#include "instruction.h"


instruction parseOpFunct(WORD_32bit instr);
instruction parseOpAddi(WORD_32bit instr);
instruction parseOpLw(WORD_32bit instr);
instruction parseOpLh(WORD_32bit instr);
instruction parseOpLhu(WORD_32bit instr);
instruction parseOpLb(WORD_32bit instr);
instruction parseOpLbu(WORD_32bit instr);
instruction parseOpSw(WORD_32bit instr);
instruction parseOpSh(WORD_32bit instr);
instruction parseOpSb(WORD_32bit instr);
instruction parseOpLui(WORD_32bit instr);
instruction parseOpAndi(WORD_32bit instr);
instruction parseOpOri(WORD_32bit instr);
instruction parseOpNori(WORD_32bit instr);
instruction parseOpSlti(WORD_32bit instr);
instruction parseOpBeq(WORD_32bit instr);
instruction parseOpBne(WORD_32bit instr);
instruction parseOpJ(WORD_32bit instr);
instruction parseOpJal(WORD_32bit instr);
instruction parseOpHalt(WORD_32bit instr);

WORD_32bit getOpcode(WORD_32bit instr);
WORD_32bit getRs(WORD_32bit instr);
WORD_32bit getRt(WORD_32bit instr);
WORD_32bit getCi(WORD_32bit instr);
WORD_32bit getCa(WORD_32bit instr);

#endif
