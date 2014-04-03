#include "simulator.h"

namespace Simulator {
    uint_32t_word Simulator::signExtend16(uint_32t_word code) {
        if ((code >> 15)&1) {
            return code | 0xFFFF0000;
        }
        return code;
    }
    bool Simulator::getSign(int number) {
        if ((number >> 31) & 1) {
            return false;
        }
        return true;
    }

} // namespace Simulator
