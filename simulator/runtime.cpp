#include "runtime.h"

/*
 * parseInstr will analyze the opcode and pass instruction
 * to next level to parse and action.
 * It will parse the instruction first and then execute it.
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

/*
 * executeInstr will execute the instruction and return 
 * the status of runtime.
 * */

WORD_32bit executeInstr(instruction tmpInstr) {
    WORD_32bit runtime_status = 0;
    switch(tmpInstr.opcode) {
        case op_funct:
            runtime_status = executeOpFunct(tmpInstr);
            break;
        case op_addi:
            runtime_status = executeOpAddi(tmpInstr);
            break;
        case op_lw:
            runtime_status = executeOpLw(tmpInstr);
            break;
        case op_lh:
            runtime_status = executeOpLh(tmpInstr);
            break;
        case op_lhu:
            runtime_status = executeOpLhu(tmpInstr);
            break;
        case op_lb:
            runtime_status = executeOpLb(tmpInstr);
            break;
        case op_lbu:
            runtime_status = executeOpLbu(tmpInstr);
            break;
        case op_sw:
            runtime_status = executeOpSw(tmpInstr);
            break;
        case op_sh:
            runtime_status = executeOpSh(tmpInstr);
            break;
        case op_sb:
            runtime_status = executeOpSb(tmpInstr);
            break;
        case op_lui:
            runtime_status = executeOpLui(tmpInstr);
            break;
        case op_andi:
            runtime_status = executeOpAndi(tmpInstr);
            break;
        case op_ori:
            runtime_status = executeOpOri(tmpInstr);
            break;
        case op_nori:
            runtime_status = executeOpNori(tmpInstr);
            break;
        case op_slti:
            runtime_status = executeOpSlti(tmpInstr);
            break;
        case op_beq:
            runtime_status = executeOpBeq(tmpInstr);
            break;
        case op_bne:
            runtime_status = executeOpBne(tmpInstr);
            break;
        case op_j:
            runtime_status = executeOpJ(tmpInstr);
            break;
        case op_jal:
            runtime_status = executeOpJal(tmpInstr);
            break;
        case op_halt:
            runtime_status = executeOpHalt(tmpInstr);
            break;
        default:
            break;
    }
    return runtime_status;
}

