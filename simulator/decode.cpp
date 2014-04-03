#include "simulator.h"

namespace Simulator {
    char Simulator::getInstructionType(uint_32t_word code) {
        uint_32t_word opcode = code >> 26;
        char type;
        switch (opcode) {
            case op_funct:
                type = 'R';
                break;
            case op_addi:   case op_lw:
            case op_lh:     case op_lhu:
            case op_lb:     case op_lbu:
            case op_sw:     case op_sh:     
            case op_sb:     case op_lui:    
            case op_andi:   case op_ori:    
            case op_nori:   case op_slti:   
            case op_beq:    case op_bne:
                type = 'I';
                break;
            case op_j:      case op_jal:
                type = 'J';
                break;
            default:
                type = 'S';
                break;
        }
        return type;
    }

    instruction Simulator::parseRType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.rs = ((code << 6) >> 6) >> 21;
        instr.rt = ((code << 11) >> 11) >> 16;
        instr.rd = ((code << 16) >> 16) >> 11;
        instr.cs = ((code << 21) >> 21) >> 6;
        instr.funct = code & 0x0000003F;
        return instr;
    }

    instruction Simulator::parseIType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.rs = ((code << 6) >> 6) >> 21;
        instr.rt = ((code << 11) >> 11) >> 16;
        instr.ci = code & 0x0000FFFF;
        return instr;
    }

    instruction Simulator::parseJType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.ca = code & 0x03FFFFFF;
        return instr;
    }
    instruction Simulator::parseSType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        return instr;
    }
} // namespace Simulator
