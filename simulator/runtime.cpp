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
            tmpInstr = parseOpRType(instr);
            break;
        case op_addi:
        case op_lw:
        case op_lh:
        case op_lhu:
        case op_lb:
        case op_lbu:
        case op_sw:
        case op_sh:
        case op_sb:
        case op_lui:
        case op_andi:
        case op_ori:
        case op_nori:
        case op_slti:
        case op_beq:
        case op_bne:
            tmpInstr = parseOpIType(instr);
            break;
        case op_j:
        case op_jal:
            tmpInstr = parseOpJType(instr);
            break;
        case op_halt:
            tmpInstr = parseOpSType(instr);
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

