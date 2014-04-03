#include "simulator.h"

namespace Simulator {
    void Simulator::_nor(instruction instr) {
        reg[instr.rd] = ~(reg[instr.rs] | reg[instr.rt]);
    }

    void Simulator::_or(instruction instr) {
        reg[instr.rd] = (reg[instr.rs] | reg[instr.rt]);
    }

    void Simulator::_xor(instruction instr) {
        reg[instr.rd] = (reg[instr.rs] ^ reg[instr.rt]);
    }

    void Simulator::_and(instruction instr) {
        reg[instr.rd] = (reg[instr.rs] & reg[instr.rt]);
    }

    void Simulator::_nand(instruction instr) {
        reg[instr.rd] = ~(reg[instr.rs] & reg[instr.rt]);
    }

    void Simulator::_slt(instruction instr) {
        reg[instr.rd] = (((int) reg[instr.rs]) < ((int) reg[instr.rt]))?
                        (1):(0);
    }

    void Simulator::_jr(instruction instr) {
        pc = reg[instr.rs];
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

    void Simulator::_sub(instruction instr) {
        int s = (int)reg[instr.rs];
        int t = (int)reg[instr.rt];
        int d = s-t;

        // Check number overflow
        if ((reg[instr.rt] == 0x8000000) ||
            ((getSign(s) == getSign(-t)) && (getSign(s) != getSign(d)))) {
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
