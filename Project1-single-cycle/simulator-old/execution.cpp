#include "execution.h"

WORD_32bit executeOpFunct(instruction tmpInstr) {
    WORD_32bit runtime_status = 0;
    switch(tmpInstr.funct) {
        case funct_add:
            runtime_status = executeFunctAdd(tmpInstr);
            break;
        case funct_sub:
            runtime_status = executeFunctSub(tmpInstr);
            break;
        case funct_and:
            runtime_status = executeFunctAnd(tmpInstr);
            break;
        case funct_or:
            runtime_status = executeFunctOr(tmpInstr);
            break;
        case funct_xor:
            runtime_status = executeFunctXor(tmpInstr);
            break;
        case funct_nor:
            runtime_status = executeFunctNor(tmpInstr);
            break;
        case funct_nand:
            runtime_status = executeFunctNand(tmpInstr);
            break;
        case funct_slt:
            runtime_status = executeFunctSlt(tmpInstr);
            break;
        case funct_sll:
            runtime_status = executeFunctSll(tmpInstr);
            break;
        case funct_srl:
            runtime_status = executeFunctSrl(tmpInstr);
            break;
        case funct_sra:
            runtime_status = executeFunctSra(tmpInstr);
            break;
        case funct_jr:
            runtime_status = executeFunctJr(tmpInstr);
            break;
        default:
            break;
    }
    return runtime_status;
}

WORD_32bit executeOpAddi(instruction tmpInstr) {
    if (tmpInstr.ci & 01000000000000000) {
        reg[tmpInstr.rt] = (int)reg[tmpInstr.rs] - (int)tmpInstr.ci;
    }
    else {
        reg[tmpInstr.rt] = (int)reg[tmpInstr.rs] + (int)tmpInstr.ci;
    }
    return status_success;
}
WORD_32bit executeOpLw(instruction tmpInstr) {
    // TODO
}
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
WORD_32bit executeFunct(instruction tmpInstr);
WORD_32bit executeFunctAdd(instruction tmpInstr);
WORD_32bit executeFunctSub(instruction tmpInstr);
WORD_32bit executeFunctAnd(instruction tmpInstr);
WORD_32bit executeFunctOr(instruction tmpInstr);
WORD_32bit executeFunctXor(instruction tmpInstr);
WORD_32bit executeFunctNor(instruction tmpInstr);
WORD_32bit executeFunctNand(instruction tmpInstr);
WORD_32bit executeFunctSlt(instruction tmpInstr);
WORD_32bit executeFunctSll(instruction tmpInstr);
WORD_32bit executeFunctSrl(instruction tmpInstr);
WORD_32bit executeFunctSra(instruction tmpInstr);
WORD_32bit executeFunctjr(instruction tmpInstr);
