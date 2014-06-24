#include "simulator.h"
//#define DEBUG

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
#ifdef DEBUG
        printf("Cycle:\t%d\n", cycleCounter);
        printf("Opcode:\t0x%02X\n", instr.op);
        printf("RegS:\t%02d\n", instr.rs);
        printf("RegT:\t%02d\n", instr.rt);
        printf("RegD:\t%02d\n", instr.rd);
        printf("Shamt:\t0x%02X\n", instr.cs);
        printf("Funct:\t0x%02X\n", instr.funct);
        printf("================================\n");
#endif
        return instr;
    }

    instruction Simulator::parseIType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.rs = ((code << 6) >> 6) >> 21;
        instr.rt = ((code << 11) >> 11) >> 16;
        instr.ci = code & 0x0000FFFF;
#ifdef DEBUG
        printf("Cycle:\t%d\n", cycleCounter);
        printf("Opcode:\t0x%02X\n", instr.op);
        printf("RegS:\t%02d\n", instr.rs);
        printf("RegT:\t%02d\n", instr.rt);
        printf("Ci:\t0x%04X\n", instr.ci);
        switch (instr.op) {
            case 0x2B:
            case 0x29:
            case 0x28:
            case 0x23:
            case 0x24:
            case 0x21:
            case 0x25:
            case 0x20:
                printf("RegS+C:\t%04d\n", reg[instr.rs] + signExtend16(instr.ci));
                printf("RegTV:\t0x%08X\n", reg[instr.rt]);
                printf("Mem[]:\t0x%08X\n", dmemory[(reg[instr.rs] + signExtend16(instr.ci))/4]);
                break;
            default:
                break;
        }
        printf("================================\n");
#endif
        return instr;
    }

    instruction Simulator::parseJType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.ca = code & 0x03FFFFFF;
#ifdef DEBUG
        printf("Cycle:\t%d\n", cycleCounter);
        printf("Opcode:\t0x%02X\n", instr.op);
        printf("Ca:\t0x%07X\n", instr.ca);
        printf("================================\n");
#endif
        return instr;
    }
    instruction Simulator::parseSType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
#ifdef DEBUG
        printf("Cycle:\t%d\n", cycleCounter);
        printf("Opcode:\t0x%02X\n", instr.op);
        printf("Halt\n");
        printf("================================\n");
#endif
        return instr;
    }
} // namespace Simulator
