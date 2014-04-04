#include "simulator.h"

namespace Simulator {
    uint_32t_word Simulator::signExtend16(uint_32t_word code) {
        if ((code >> 15)&1) {
            return code | 0xFFFF0000;
        }
        return code;
    }
    uint_32t_word Simulator::signExtend8(uint_32t_word code) {
        if ((code >> 7)&1) {
            return code | 0xFFFFFF00;
        }
        return code;
    }
    bool Simulator::getSign(int number) {
        if ((number >> 31) & 1) {
            return false;
        }
        return true;
    }
    void Simulator::checkWriteToRegZeroError(uint_32t_word regNumber) {
        if (regNumber == 0) {
            fprintf(errordump, "Write $0 error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_SKIP;
        }
    }

} // namespace Simulator
