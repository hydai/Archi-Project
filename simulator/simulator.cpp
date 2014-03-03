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
#ifndef DEBUG 
    while (1) {
        /* print data */
        memory_dump(snapshot);
        /* increase pc */
        pc = pc+1;
    }
#else
    memory_dump(snapshot);
#endif
    /* Close file pointers and exit simulator */
    fclose(iimage);
    fclose(dimage);
    fclose(snapshot);
    fclose(error_dump);
    return 0;
}

