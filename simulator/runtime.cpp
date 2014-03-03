#include "runtime.h"

/*
 * parseInstr will analyze the opcode and pass instruction
 * to next level to parse the action. And it will return 
 * an data structure called "instruction". Then excution
 * part will take the "instruction" to finish the work.
 */
instruction parseInstr(WORD_32bit instr) {
    instruction tmpInstr;
    switch(getOpcode(instr)) {
        case op_funct:
            tmpInstr = parseOpFunct(instr);
            break;
        case op_addi:
            tmpInstr = parseOpAddi(instr);
            break;
        case op_lw:
            tmpInstr = parseOpLw(instr);
            break;
        case op_lh:
            tmpInstr = parseOpLh(instr);
            break;
        case op_lhu:
            tmpInstr = parseOpLhu(instr);
            break;
        case op_lb:
            tmpInstr = parseOpLb(instr);
            break;
        case op_lbu:
            tmpInstr = parseOpLbu(instr);
            break;
        case op_sw:
            tmpInstr = parseOpSw(instr);
            break;
        case op_sh:
            tmpInstr = parseOpSh(instr);
            break;
        case op_sb:
            tmpInstr = parseOpSb(instr);
            break;
        case op_lui:
            tmpInstr = parseOpLui(instr);
            break;
        case op_andi:
            tmpInstr = parseOpAndi(instr);
            break;
        case op_ori:
            tmpInstr = parseOpOri(instr);
            break;
        case op_nori:
            tmpInstr = parseOpNori(instr);
            break;
        case op_slti:
            tmpInstr = parseOpSlti(instr);
            break;
        case op_beq:
            tmpInstr = parseOpBeq(instr);
            break;
        case op_bne:
            tmpInstr = parseOpBne(instr);
            break;
        case op_j:
            tmpInstr = parseOpJ(instr);
            break;
        case op_jal:
            tmpInstr = parseOpJal(instr);
            break;
        case op_halt:
            tmpInstr = parseOpHalt(instr);
            break;
        default:
            break;
    }
    return tmpInstr;
}
