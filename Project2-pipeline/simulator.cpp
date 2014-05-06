#include "simulator.h"
namespace Simulator {
    Simulator::Simulator() {
        this->init();
    }

    Simulator::~Simulator() {
        this->fileClose();
    }

    void Simulator::run() {
        while (true) {
            uint_32t_word currentInstruction = 0;
            instruction instr;
            // dump status
            this->dump();
            /*
            // fetch opcode
            currentInstruction = this->fetch();
            if (runtimeStatus == STATUS_HALT) {
            break;
            }
            // decode
            instr = this->decode(currentInstruction);
            // execution
            this->execute(instr);
            if (runtimeStatus == STATUS_HALT) {
            break;
            }
            */
            runtimeStatus = STATUS_NORMAL;
            break;
        }
    }
} // namespace Simulator
