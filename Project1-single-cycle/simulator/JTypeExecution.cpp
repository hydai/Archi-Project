#include "simulator.h"
namespace Simulator {
    void Simulator::_j(instruction instr) {
        uint_32t_word tmp = 0;
        tmp = tmp | (pc & 0xF0000000);
        tmp = tmp | (instr.ca << 2);
        pc = tmp;
    }
    void Simulator::_jal(instruction instr) {
        reg[31] = pc;
        this->_j(instr);
    }
} // namespace Simulator
