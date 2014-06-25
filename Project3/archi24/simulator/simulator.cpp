#include "simulator.h"
#include <cstring>

namespace Simulator {
    Simulator::Simulator(
      int IMSIZE,       // I memory size
      int DMSIZE,       // D memory size
      int IPGSIZE,      // Page size of I memory
      int DPGSIZE,      // Page size of D memory
      int ICTSIZE,      // Total size of I cache
      int ICBSIZE,      // Block size of I cache
      int ICSA,         // Set associativity of I cache
      int DCTSIZE,      // Total size of D cache
      int DCBSIZE,      // Block size of D cache
      int DCSA          // Set associativity of D cache
      ) {
        this->init();
        this->loadData();
        // init for report units
        memset(IMEM.page, 0, sizeof(IMEM.page));
        memset(DMEM.page, 0, sizeof(DMEM.page));
        memset(ICACHE.block, 0, sizeof(ICACHE.block));
        memset(DCACHE.block, 0, sizeof(DCACHE.block));
        memset(ITLB.ppn, 0, sizeof(ITLB.ppn));
        memset(DTLB.ppn, 0, sizeof(DTLB.ppn));
        memset(IPTE.ppn, 0, sizeof(IPTE.ppn));
        memset(DPTE.ppn, 0, sizeof(DPTE.ppn));
        IMEM.size = IMSIZE;
        IMEM.pageSize = IPGSIZE;
        DMEM.size = DMSIZE;
        DMEM.pageSize = DPGSIZE;
        ICACHE.totalSize = ICTSIZE;
        ICACHE.blockSize = ICBSIZE;
        ICACHE.setAssociativity = ICSA;
        DCACHE.totalSize = DCTSIZE;
        DCACHE.blockSize = DCBSIZE;
        DCACHE.setAssociativity = DCSA;
        IPTE.entry = 1024/IMEM.pageSize;
        DPTE.entry = 1024/DMEM.pageSize;
        ITLB.entry = IPTE.entry/4;
        DTLB.entry = DPTE.entry/4;
    }
    Simulator::Simulator() {
        this->init();
        this->loadData();
        // init for report units
        memset(IMEM.page, 0, sizeof(IMEM.page));
        memset(DMEM.page, 0, sizeof(DMEM.page));
        memset(ICACHE.block, 0, sizeof(ICACHE.block));
        memset(DCACHE.block, 0, sizeof(DCACHE.block));
        memset(ITLB.ppn, 0, sizeof(ITLB.ppn));
        memset(DTLB.ppn, 0, sizeof(DTLB.ppn));
        memset(IPTE.ppn, 0, sizeof(IPTE.ppn));
        memset(DPTE.ppn, 0, sizeof(DPTE.ppn));
        IMEM.size = 64;
        IMEM.pageSize = 8;
        DMEM.size = 32;
        DMEM.pageSize = 16;
        ICACHE.totalSize = 16;
        ICACHE.blockSize = 4;
        ICACHE.setAssociativity = 4;
        DCACHE.totalSize = 16;
        DCACHE.blockSize = 4;
        DCACHE.setAssociativity = 1;
        IPTE.entry = 1024/IMEM.pageSize;
        DPTE.entry = 1024/DMEM.pageSize;
        ITLB.entry = IPTE.entry/4;
        DTLB.entry = DPTE.entry/4;
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
        runtimeStatus = STATUS_NORMAL;
    }
    bool Simulator::loadData() {
        // open files, if something goes wrong, return false
        iimage = fopen("iimage.bin", "r");
        dimage = fopen("dimage.bin", "r");
        errordump = fopen("error_dump.rpt", "w");
        snapshot = fopen("snapshot.rpt", "w");
        fptr_report = fopen("report.rpt", "w");
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
    void Simulator::report() {
        fprintf( fptr_report, "ICache :\n");
        fprintf( fptr_report, "# hits: %u\n", ICACHE.hits );
        fprintf( fptr_report, "# misses: %u\n\n", ICACHE.misses );
        fprintf( fptr_report, "DCache :\n");
        fprintf( fptr_report, "# hits: %u\n", DCACHE.hits );
        fprintf( fptr_report, "# misses: %u\n\n", DCACHE.misses );
        fprintf( fptr_report, "ITLB :\n");
        fprintf( fptr_report, "# hits: %u\n", ITLB.hits );
        fprintf( fptr_report, "# misses: %u\n\n", ITLB.misses );
        fprintf( fptr_report, "DTLB :\n");
        fprintf( fptr_report, "# hits: %u\n", DTLB.hits );
        fprintf( fptr_report, "# misses: %u\n\n", DTLB.misses );
        fprintf( fptr_report, "IPageTable :\n");
        fprintf( fptr_report, "# hits: %u\n", IPTE.hits );
        fprintf( fptr_report, "# misses: %u\n\n", IPTE.misses );
        fprintf( fptr_report, "DPageTable :\n");
        fprintf( fptr_report, "# hits: %u\n", DPTE.hits );
        fprintf( fptr_report, "# misses: %u\n\n", DPTE.misses );
    }
    void Simulator::run() {
        while (true) {
            uint_32t_word currentInstruction = 0;
            instruction instr;
            // dump status
            this->dump();
            // fetch opcode
            currentInstruction = this->fetch();
            if (runtimeStatus == STATUS_HALT) {
                break;
            }
            // decode
            instr = this->decode(currentInstruction);
            // execution
            this->execute(instr);
            if (runtimeStatus == STATUS_HALT) {
                break;
            }
            runtimeStatus = STATUS_NORMAL;
        }
        this->report();
    }
    uint_32t_word Simulator::fetch() {
        uint_32t_word opcode;
        bool hasError = false;

        // Check address overflow
        if (pc >= 1024) {
            fprintf(errordump, "Address overflow in cycle: %d\n", cycleCounter);
            hasError = true;
        }
        // Check misalignment error
        if (pc % 4 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            hasError = true;
        }

        // check if the error happens or not
        if (hasError) {
            runtimeStatus = STATUS_HALT;
            return 0;
        }

        // get opcode
        opcode = imemory[pc/4];
        // PC move to next instruction
        pc += 4;
        return opcode;
    }

    instruction Simulator::decode(uint_32t_word code) {
        instruction instr;
        switch (getInstructionType(code)) {
            case 'R':
                instr = parseRType(code);
                break;
            case 'I':
                instr = parseIType(code);
                break;
            case 'J':
                instr = parseJType(code);
                break;
            default:
                // halt instruction
                instr = parseSType(code);
                break;
        }
        return instr;
    }

    void Simulator::execute(instruction instr) {
        switch (instr.op) {
            case op_funct:
                _funct(instr);
                break;
            case op_addi:
                _addi(instr);
                break;
            case op_lw:
                _lw(instr);
                break;
            case op_lh:
                _lh(instr);
                break;
            case op_lhu:
                _lhu(instr);
                break;
            case op_lb:
                _lb(instr);
                break;
            case op_lbu:
                _lbu(instr);
                break;
            case op_sw:
                _sw(instr);
                break;
            case op_sh:
                _sh(instr);
                break;
            case op_sb:
                _sb(instr);
                break;
            case op_lui:
                _lui(instr);
                break;
            case op_andi:
                _andi(instr);
                break;
            case op_ori:
                _ori(instr);
                break;
            case op_nori:
                _nori(instr);
                break;
            case op_slti:
                _slti(instr);
                break;
            case op_beq:
                _beq(instr);
                break;
            case op_bne:
                _bne(instr);
                break;
            case op_j:
                _j(instr);
                break;
            case op_jal:
                _jal(instr);
                break;
            case op_halt:
                runtimeStatus = STATUS_HALT;
                break;
            default:
                break;
        }
    }
    void Simulator::_funct(instruction instr) {
        switch (instr.funct) {
            case funct_add:
                _add(instr);
                break;
            case funct_sub:
                _sub(instr);
                break;
            case funct_and:
                _and(instr);
                break;
            case funct_or:
                _or(instr);
                break;
            case funct_xor:
                _xor(instr);
                break;
            case funct_nor:
                _nor(instr);
                break;
            case funct_nand:
                _nand(instr);
                break;
            case funct_slt:
                _slt(instr);
                break;
            case funct_sll:
                _sll(instr);
                break;
            case funct_srl:
                _srl(instr);
                break;
            case funct_sra:
                _sra(instr);
                break;
            case funct_jr:
                _jr(instr);
                break;
            default:
                break;
        }
    }
} // namespace Simulator
