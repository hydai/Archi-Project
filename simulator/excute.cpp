#include "simulator.h"

namespace Simulator {
    uint_32t_word Simulator::signExtend(uint_32t_word code) {
        if ((code >> 15)&1) {
            return code | 0xFFFF0000;
        }
        return code;
    }
    bool getSign(int number) {
        if ((number >> 31) & 1) {
            return false;
        }
        return true;
    }
    void Simulator::_lw(instruction instr) {
        int offset = (int) signExtend(instr.ci);
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

    void Simulator::_nor(instruction instr) {
        reg[instr.rd] = ~(reg[instr.rs] | reg[instr.rt]);
    }

    void Simulator::_or(instruction instr) {
        reg[instr.rd] = (reg[instr.rs] | reg[instr.rt]);
    }

    void Simulator::_add(instruction instr) {
        int s = (int)reg[instr.rs];
        int t = (int)reg[instr.rt];
        int d = s+t;
        // Check number overflow
        if ((getSign(s) == getSign(t)) && (getSign(s) != getSign(d))) {
            fprintf(errordump, "Number overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rd] = (uint_32t_word)d;
    }
} // namespace Simulator
