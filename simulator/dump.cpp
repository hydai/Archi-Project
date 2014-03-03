#include "dump.h"

void memory_dump(FILE *snapshot) {
    fprintf(snapshot, "cycle %u\n", cycle_counter);
    for (int i = 0; i < REGISTER_SIZE; i++) {
        fprintf(snapshot, "$%02d: 0x%08X\n", i, reg[i]);
    }
    fprintf(snapshot, "PC: 0x%08X\n", printPC(pc));
    fprintf(snapshot, "\n\n");
}

WORD_32bit printPC(WORD_32bit simuPC) {
    return simuPC*4;
}
