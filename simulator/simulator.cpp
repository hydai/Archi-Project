#include "simulator.h"

namespace Simulator {
    Simulator::Simulator() {
        this->init();
        this->loadData();
    }
    Simulator::~Simulator() {
        fclose(errordump);
        fclose(snapshot);
    }
    void Simulator::init() {
        memset(imemory, 0, sizeof(imemory));
        memset(dmemory, 0, sizeof(dmemory));
        memset(reg, 0, sizeof(reg));
        pc = 0;
        cycleCounter = 0;
    }
    bool Simulator::loadData() {
        // open files, if something goes wrong, return false
        iimage = fopen("iimage.bin", "r");
        dimage = fopen("dimage.bin", "r");
        errordump = fopen("error_dump.rpt", "w");
        snapshot = fopen("snapshot.rpt", "w");
        if (iimage == NULL || dimage == NULL) {
            printf("Cannot open iimage.bin or dimage.bin\n");
            return false;
        }
        
        this->loadIimage();
        this->loadDimage();
        return true;
    }
    void Simulator::loadIimage() {
        // load iimage
        // get pc
        fread(&pc, sizeof(uint_32t_word), 1, iimage);
        // get cycle number
        uint_32t_word cycleNumber;
        fread(&cycleNumber, sizeof(uint_32t_word), 1, iimage);
        // get instructions
        for (int i = 0; i < cycleNumber; i++) {
            fread((imemory+(pc/4)+i), sizeof(uint_32t_word), 1, iimage);
        }
        fclose(iimage);
    }
    void Simulator::loadDimage() {
        // get reg $sp
        fread(&reg[29], sizeof(uint_32t_word), 1, dimage);
        // get word number
        uint_32t_word wordNumber;
        fread(&wordNumber, sizeof(uint_32t_word), 1, dimage);
        // get data
        for (int i = 0; i < wordNumber; i++) {
            fread((dmemory+i), sizeof(uint_32t_word), 1, dimage);
        }
        fclose(dimage);
    }
    void Simulator::dump() {
        fprintf(snapshot, "cycle %u\n", cycleCounter++);
        for (int i = 0; i < REG_SIZE; i++) {
            fprintf(snapshot, "$%02d: 0x%08X\n", i, reg[i]);
        }
        fprintf(snapshot, "PC: 0x%08X\n", pc);
        fprintf(snapshot, "\n\n");
    }
    void Simulator::run() {
        while (true) {
            // dump status
            this->dump();
            // fetch opcode
            // decode
            // excution

        }
    }
} // namespace Simulator
