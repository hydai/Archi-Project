#include "simulator.h"

namespace Simulator {
    Simulator::Simulator() {
        this->init();
        this->loadData();
    }
    void Simulator::init() {
        memset(imemory, 0, sizeof(imemory));
        memset(dmemory, 0, sizeof(dmemory));
        memset(reg, 0, sizeof(reg));
        pc = 0;
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
    }
    void Simulator::loadIimage() {
        // load iimage
        // get pc
        fread(&pc, sizeof(uint_32t), 1, iimage);
        // get cycle number
        uint_32t cycleNumber;
        fread(&cycleNumber, sizeof(uint_32t), 1, iimage);
        // get instructions
        for (int i = 0; i < cycleNumber; i++) {
            fread((imemory+pc+i), sizeof(uint_32t), 1, iimage);
        }
    }
    void Simulator::loadDimage() {
        // get reg $sp
        fread(&reg[29], sizeof(uint_32t), 1, dimage);
        // get word number
        uint_32t wordNumber;
        fread(&wordNumber, sizeof(uint_32t), 1, dimage);
        // get data
        for (int i = 0; i < cycleNumber; i++) {
            fread((dmemory+i), sizeof(uint_32t), 1, dimage);
        }
    }
} // namespace Simulator
