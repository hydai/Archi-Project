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
#include "runtime.h"
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
    /* print initial data */
    memory_dump(snapshot);
    while (1) {
        /* get current instruction */
        WORD_32bit currentInstr = imemory[transformInitialPCtoSimulatorPC(pc)];
        /* increase pc */
        pc = pc+4;

        /* run the instruction */
        instruction tmpInstr = parseInstr(currentInstr);
        WORD_32bit runtimeStatus = executeInstr(tmpInstr);
        
        if (runtimeStatus == status_halt) {
            break;
        }

        /* print data */
        memory_dump(snapshot);
    }
    /* Close file pointers and exit simulator */
    fclose(iimage);
    fclose(dimage);
    fclose(snapshot);
    fclose(error_dump);
    return 0;
}

