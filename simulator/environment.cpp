#include "environment.h"
#include <cstring>

WORD_32bit reg[REGISTER_SIZE];
WORD_32bit imemory[MEMORY_SIZE];
WORD_32bit dmemory[MEMORY_SIZE];
WORD_32bit cycle_counter;
WORD_32bit pc;

/*
 * Initial the simulator.
 * All memory should be set to zero.
 * PC and cycle counter are both set to zero.
 */
void init() {
    memset(imemory, 0, sizeof(imemory));
    memset(dmemory, 0, sizeof(dmemory));
    cycle_counter = 0;
    pc = 0;
}
WORD_32bit transformInitialPCtoSimulatorPC(WORD_32bit initPC) {
    return initPC/4;
}
/*
 * Load the pc counter and cycle_number
 */
void load_basic_setting(FILE *iimage) {
    fread(&pc, sizeof(WORD_32bit), 1, iimage);
}
void load_imemory(FILE *iimage) {
    WORD_32bit cycle_number;
    fread(&cycle_number, sizeof(WORD_32bit), 1, iimage);
    for (int i = 0; i < cycle_number; i++) {
        fread((imemory+pc+i), sizeof(WORD_32bit), 1, iimage);
    }
}
void load_dmemory(FILE *dimage) {
    WORD_32bit dimage_word_num;
    /* read $sp from dimage */
    fread(&(reg[sp]), sizeof(WORD_32bit), 1, dimage);
    fread(&dimage_word_num, sizeof(WORD_32bit), 1, dimage);
    for (int i = 0; i < dimage_word_num; i++) {
        fread((dmemory+i), sizeof(WORD_32bit), 1, dimage);
    }
}
