#include "simulator.h"

namespace Simulator {
    void Simulator::_nor(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = ~(reg[instr.rs] | reg[instr.rt]);
    }

    void Simulator::_or(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = (reg[instr.rs] | reg[instr.rt]);
    }

    void Simulator::_xor(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = (reg[instr.rs] ^ reg[instr.rt]);
    }

    void Simulator::_and(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = (reg[instr.rs] & reg[instr.rt]);
    }

    void Simulator::_nand(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = ~(reg[instr.rs] & reg[instr.rt]);
    }

    void Simulator::_slt(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = (((int) reg[instr.rs]) < ((int) reg[instr.rt]))?
                        (1):(0);
    }

    void Simulator::_jr(instruction instr) {
        pc = reg[instr.rs];
    }

    void Simulator::_sll(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = reg[instr.rt] << instr.cs;
    }

    void Simulator::_srl(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = reg[instr.rt] >> instr.cs;
    }

    void Simulator::_sra(instruction instr) {
        uint_32t_word mask = (getSign(reg[instr.rt]))?(0):(0x80000000);
        uint_32t_word tmp = reg[instr.rt];
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        for (int i = 0; i < instr.cs; i++) {
            tmp = tmp >> 1;
            tmp = tmp | mask;
        }
        reg[instr.rd] = tmp;
    }

    void Simulator::_add(instruction instr) {
        int s = (int)reg[instr.rs];
        int t = (int)reg[instr.rt];
        int d = s+t;
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // Check number overflow
        if ((getSign(s) == getSign(t)) && (getSign(s) != getSign(d))) {
            fprintf(errordump, "Number overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = (runtimeStatus == STATUS_NORMAL)?STATUS_CONTINUE:runtimeStatus;
        }
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = (uint_32t_word)d;
    }

    void Simulator::_sub(instruction instr) {
        int s = (int)reg[instr.rs];
        int t = (int)reg[instr.rt];
        int d = s-t;

        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rd);
        // Check number overflow
        if ((reg[instr.rt] == 0x80000000) ||
            ((getSign(s) == getSign(-t)) && (getSign(s) != getSign(d)))) {
            fprintf(errordump, "Number overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = (runtimeStatus == STATUS_NORMAL)?STATUS_CONTINUE:runtimeStatus;
        }
        // check if the error happens or not
        if (runtimeStatus == STATUS_HALT || runtimeStatus == STATUS_SKIP) {
            return;
        }
        reg[instr.rd] = (uint_32t_word)d;
    }
} // namespace Simulator
