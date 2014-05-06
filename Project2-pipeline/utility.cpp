#include "simulator.h"
namespace Simulator {
    void Simulator::init() {
        memset(imemory, 0, sizeof(imemory));
        memset(dmemory, 0, sizeof(dmemory));
        memset(reg, 0, sizeof(reg));
        pc = 0;
        cycleCounter = 0;
        runtimeStatus = STATUS_NORMAL;
        this->load();
    }
    void Simulator::dump() {
        fprintf(snapshot, "cycle %u\n", cycleCounter++);
        for (int i = 0; i < REG_SIZE; i++) {
            fprintf(snapshot, "$%02d: 0x%08X\n", i, reg[i]);
        }
        fprintf(snapshot, "PC: 0x%08X\n", pc);
        fprintf(snapshot, "\n\n");
    }
} // namespace Simulator
