#ifndef PARSE_H
#define PARSE_H

#include "const.h"
#include "instruction.h"


instruction parseOpRType(WORD_32bit instr);
instruction parseOpIType(WORD_32bit instr);
instruction parseOpJType(WORD_32bit instr);
instruction parseOpSType(WORD_32bit instr);


WORD_32bit getOpcode(WORD_32bit instr);
WORD_32bit getRs(WORD_32bit instr);
WORD_32bit getRt(WORD_32bit instr);
WORD_32bit getCi(WORD_32bit instr);
WORD_32bit getCa(WORD_32bit instr);

#endif
