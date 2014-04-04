#include "simulator.h"

namespace Simulator {
    void Simulator::_bne(instruction instr) {
        if (reg[instr.rs] != reg[instr.rt]) {
            pc = pc + (signExtend16(instr.ci)*4);
        }
    }

    void Simulator::_beq(instruction instr) {
        if (reg[instr.rs] == reg[instr.rt]) {
            pc = pc + (signExtend16(instr.ci)*4);
        }
    }
    
    void Simulator::_andi(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rt);

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }

        reg[instr.rt] = reg[instr.rs] & instr.ci;
    }

    void Simulator::_ori(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rt);

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }

        reg[instr.rt] = reg[instr.rs] | instr.ci;
    }

    void Simulator::_nori(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rt);

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }

        reg[instr.rt] = ~(reg[instr.rs] | instr.ci);
    }

    void Simulator::_lui(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rt);

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }

        reg[instr.rt] = instr.ci << 16;
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
        this->checkWriteToRegZeroError(instr.rt);
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
        if ((base + offset) % 2 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = signExtend16(dmemory[(base+offset)/4] & 0x0000FFFF);
    }

    void Simulator::_lb(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

        // write to $0
        this->checkWriteToRegZeroError(instr.rt);
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

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = signExtend8(dmemory[(base+offset)/4] & 0x000000FF);
    }

    void Simulator::_lbu(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

        // write to $0
        this->checkWriteToRegZeroError(instr.rt);
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

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = dmemory[(base+offset)/4] & 0x000000FF;
    }

    void Simulator::_sw(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

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
        dmemory[(base+offset)/4] = reg[instr.rt];
        
    }

    void Simulator::_sh(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

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
        if ((base + offset) % 2 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        dmemory[(base+offset)/4] = (dmemory[(base+offset)/4] & 0xFFFF0000)
                                    || (reg[instr.rt] & 0x0000FFFF);
    }

    void Simulator::_sb(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

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

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        dmemory[(base+offset)/4] = (dmemory[(base+offset)/4] & 0xFFFFFF00)
                                    || (reg[instr.rt] & 0x000000FF);
    }

    void Simulator::_lw(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

        // write to $0
        this->checkWriteToRegZeroError(instr.rt);
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
        reg[instr.rt] = dmemory[(base+offset)/4];
    }

    void Simulator::_lhu(instruction instr) {
        int offset = (int) signExtend16(instr.ci);
        int base = (int) reg[instr.rs];

        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rt);
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
        if ((base + offset) % 2 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = dmemory[(base+offset)/4] & 0x0000FFFF;
    }
    void Simulator::_slti(instruction instr) {
        // Check for writing to $0
        this->checkWriteToRegZeroError(instr.rt);
        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        reg[instr.rt] = (((int) reg[instr.rs]) < ((int) signExtend16(instr.ci)))?
                        (1):(0);
    }
} // namespace Simulator
