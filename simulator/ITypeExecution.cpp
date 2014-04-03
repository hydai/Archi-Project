#include "simulator.h"

namespace Simulator {
    void Simulator::_bne(instruction instr) {
        if (reg[instr.rs] != reg[instr.rt]) {
            pc = pc + 4 + (signExtend16(instr.ci)*4);
        }
    }

    void Simulator::_beq(instruction instr) {
        if (reg[instr.rs] == reg[instr.rt]) {
            pc = pc + 4 + (signExtend16(instr.ci)*4);
        }
    }
    
    void Simulator::_addi(instruction instr) {
        int s = (int)reg[instr.rs];
        int ci = (int)signExtend16(instr.ci);

        // Check number overflow
        if ((getSign(s) == getSign(ci)) && (getSign(s) != getSign(s+ci))) {
            fprintf(errordump, "Number overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = s+ci;
    }
    void Simulator::_lh(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

        // write to $0
        if (instr.rt == 0) {
            fprintf(errordump, "Write $0 error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // Check number overflow
        if ((getSign(base) == getSign(offset)) && (getSign(base) != getSign(base+offset))) {
            fprintf(errordump, "Number overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // Check address overflow
        if (base + offset >= 1024 || base + offset < 0) {
            fprintf(errordump, "Address overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }
        // Check misalignment error
        if ((base + offset) % 4 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = signExtend16(dmemory[base+offset] & 0x0000FFFF);
    }

    void Simulator::_lw(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

        // write to $0
        if (instr.rt == 0) {
            fprintf(errordump, "Write $0 error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // Check number overflow
        if ((getSign(base) == getSign(offset)) && (getSign(base) != getSign(base+offset))) {
            fprintf(errordump, "Number overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // Check address overflow
        if (base + offset >= 1024 || base + offset < 0) {
            fprintf(errordump, "Address overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }
        // Check misalignment error
        if ((base + offset) % 4 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = dmemory[base+offset];
    }
} // namespace Simulator
