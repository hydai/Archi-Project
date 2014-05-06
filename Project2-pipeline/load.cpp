#include "simulator.h"
namespace Simulator {
    void Simulator::loadIimage() {
        // get pc
        fread(&pc, sizeof(uint_32t_word), 1, iimage);
        // get cycle number
        uint_32t_word cycleNumber;
        fread(&cycleNumber, sizeof(uint_32t_word), 1, iimage);
        // get instructions
        for (int i = 0; i < cycleNumber; i++) {
            fread((imemory+(pc/4)+i), sizeof(uint_32t_word), 1, iimage);
        }
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
    }
    void Simulator::fileOpen() {
        iimage = fopen("iimage.bin", "r");
        dimage = fopen("dimage.bin", "r");
        errordump = fopen("errordump.rpt", "w");
        snapshot = fopen("snapshot.rpt", "w");
    }
    void Simulator::fileClose() {
        fclose(iimage);
        fclose(dimage);
        fclose(errordump);
        fclose(snapshot);
    }
    void Simulator::load() {
        this->fileOpen();
        this->loadIimage();
        this->loadDimage();
    }
} // namespace Simulator
