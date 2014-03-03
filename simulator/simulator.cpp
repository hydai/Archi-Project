/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/

/*
 * This part is the main entry part of our project "Simulator".
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "const.h"
#include "environment.h"
#include "dump.h"


int main(int argc, char *argv[])
{
    /* Declare file pointers and memory part. */
    FILE *iimage, *dimage, *snapshot, *error_dump;
    iimage = fopen("iimage.bin", "r");
    dimage = fopen("dimage.bin", "r");
    snapshot = fopen("snapshot.rpt", "w");
    error_dump = fopen("error_dump.rpt", "w");

    /* Initial the basic setting */
    init();
    load_basic_setting(iimage);
    pc = transformInitialPCtoSimulatorPC(pc);
    load_imemory(iimage);
    load_dmemory(dimage);
    
    while (1) {
        /* print data */
        memory_dump(snapshot);
        /* increase pc */
        pc = pc+1;
    }

    /* Close file pointers and exit simulator */
    fclose(iimage);
    fclose(dimage);
    fclose(snapshot);
    fclose(error_dump);
    return 0;
}

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
WORD_32bit printPC(WORD_32bit simuPC) {
    return simuPC*4;
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
void memory_dump(FILE *snapshot) {
    fprintf(snapshot, "cycle %u\n", cycle_counter);
    for (int i = 0; i < REGISTER_SIZE; i++) {
        fprintf(snapshot, "$%02d: 0x%08X\n", i, reg[i]);
    }
    fprintf(snapshot, "PC: 0x%08X\n", printPC(pc));
    fprintf(snapshot, "\n\n");
}
