/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
must display the following acknowledgement:
This product includes software developed by the organization.
4. Neither the name of the organization nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Hung-Ying, Dai ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Hung-Ying, Dai BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * This part is the main entry part of our project "Simulator".
 */
#include <cstdio>
#include <cstdlib>

#define MEMORY_SIZE (256)
#define WORD_32bit (unsigned int)
WORD_32bit imemory[MEMORY_SIZE];
WORD_32bit dmemory[MEMORY_SIZE];
WORD_32bit cycle_counter;
WORD_32bit cycle_number;
WORD_32bit pc;

void init();
void load_basic_setting(FILE *iimage);
void load_imemory(FILE *iimage, WORD_32bit cycle_number);
void load_dmemory(FILE *dimage);
WORD_32bit transformInitialPCtoSimulatorPC(WORD_32bit initPC);
WORD_32bit printPC(WORD_32bit simuPC);
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
    load_imemory(iimage, cycle_number);
    load_dmemory(dimage);
    
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
 * PC counter and cycle counter are both set to zero.
 */
void init() {
    memset(imemory, 0, sizeof(imemory));
    memset(dmemory, 0, sizeof(dmemory));
    cycle_counter = 0;
    cycle_number = 0;
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
    fread(&cycle_number, sizeof(WORD_32bit), 1, iimage);
}
void load_imemory(FILE *iimage, WORD_32bit cycle_number) {
    for (int i = 0; i < cycle_number; i++) {
        fread(&(imemory+pc+i), sizeof(WORD_32bit), 1, iimage);
    }
}
void load_dmemory(FILE *dimage);
