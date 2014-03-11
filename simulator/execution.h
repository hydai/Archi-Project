#ifndef EXECUTION_H
#define EXECUTION_H

#include "const.h"
#include "instruction.h"
#include "environment.h"

WORD_32bit executeOpFunct(instruction tmpInstr);
WORD_32bit executeOpAddi(instruction tmpInstr);
WORD_32bit executeOpLw(instruction tmpInstr);
WORD_32bit executeOpLh(instruction tmpInstr);
WORD_32bit executeOpLhu(instruction tmpInstr);
WORD_32bit executeOpLb(instruction tmpInstr);
WORD_32bit executeOpLbu(instruction tmpInstr);
WORD_32bit executeOpSw(instruction tmpInstr);
WORD_32bit executeOpSh(instruction tmpInstr);
WORD_32bit executeOpSb(instruction tmpInstr);
WORD_32bit executeOpLui(instruction tmpInstr);
WORD_32bit executeOpAndi(instruction tmpInstr);
WORD_32bit executeOpOri(instruction tmpInstr);
WORD_32bit executeOpNori(instruction tmpInstr);
WORD_32bit executeOpSlti(instruction tmpInstr);
WORD_32bit executeOpBeq(instruction tmpInstr);
WORD_32bit executeOpBne(instruction tmpInstr);
WORD_32bit executeOpJ(instruction tmpInstr);
WORD_32bit executeOpJal(instruction tmpInstr);
WORD_32bit executeOpHalt(instruction tmpInstr);
WORD_32bit executeOp(instruction tmpInstr);

#endif
