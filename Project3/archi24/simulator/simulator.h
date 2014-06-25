#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace Simulator {
    const int MEMORY_SIZE = 1024;
    const int REG_SIZE = 32;
    typedef unsigned int uint_32t_word ;
    const uint_32t_word STATUS_NORMAL   = 0;
    const uint_32t_word STATUS_CONTINUE = 1;
    const uint_32t_word STATUS_SKIP     = 2;
    const uint_32t_word STATUS_HALT     = 3;
    const uint_32t_word op_funct        = 0x00;
    const uint_32t_word op_addi         = 0x08;
    const uint_32t_word op_lw           = 0x23;
    const uint_32t_word op_lh           = 0x21;
    const uint_32t_word op_lhu          = 0x25;
    const uint_32t_word op_lb           = 0x20;
    const uint_32t_word op_lbu          = 0x24;
    const uint_32t_word op_sw           = 0x2B;
    const uint_32t_word op_sh           = 0x29;
    const uint_32t_word op_sb           = 0x28;
    const uint_32t_word op_lui          = 0x0F;
    const uint_32t_word op_andi         = 0x0C;
    const uint_32t_word op_ori          = 0x0D;
    const uint_32t_word op_nori         = 0x0E;
    const uint_32t_word op_slti         = 0x0A;
    const uint_32t_word op_beq          = 0x04;
    const uint_32t_word op_bne          = 0x05;
    const uint_32t_word op_j            = 0x02;
    const uint_32t_word op_jal          = 0x03;
    const uint_32t_word op_halt         = 0x3F;

    const uint_32t_word funct_add       = 0x20;
    const uint_32t_word funct_sub       = 0x22;
    const uint_32t_word funct_and       = 0x24;
    const uint_32t_word funct_or        = 0x25;
    const uint_32t_word funct_xor       = 0x26;
    const uint_32t_word funct_nor       = 0x27;
    const uint_32t_word funct_nand      = 0x28;
    const uint_32t_word funct_slt       = 0x2A;
    const uint_32t_word funct_sll       = 0x00;
    const uint_32t_word funct_srl       = 0x02;
    const uint_32t_word funct_sra       = 0x03;
    const uint_32t_word funct_jr        = 0x08;

    struct instruction {
        uint_32t_word op;
        uint_32t_word funct;
        uint_32t_word rs, rt, rd;
        uint_32t_word ci, cs, ca;
    };
    struct _PAGE {
        int space[256];
        int LRU;
    };
    struct _MEMORY {
        int hits, misses;
        int size, pageSize;
        _PAGE page[256];
    };
    struct _BLOCK {
        bool valid;
        int tag, ind, off;
        int LRU;
    };
    struct _CACHE {
        int hits, misses;
        int totalSize, blockSize, setAssociativity;
        _BLOCK block[256][256];
    };
    struct _PPN {
        bool valid;
        int ppn;
        int LRU;
    };
    struct _TLB {
        int hits, misses;
        int entry;
        _PPN ppn[256];
    };
    struct _PTE {
        int hits, misses;
        int entry;
        _PPN ppn[256];
    };
    class Simulator {
        public:
            Simulator();
            Simulator(int, int, int, int, int, int, int, int, int, int);
            ~Simulator();
            void run();
        private:
            uint_32t_word imemory[MEMORY_SIZE/4];
            uint_32t_word dmemory[MEMORY_SIZE/4];
            uint_32t_word reg[REG_SIZE];
            uint_32t_word pc;
            uint_32t_word cycleCounter;
            uint_32t_word runtimeStatus;
            _MEMORY IMEM, DMEM;
            _CACHE ICACHE, DCACHE;
            _TLB ITLB, DTLB;
            _PTE IPTE, DPTE;
            FILE *iimage, *dimage, *errordump, *snapshot, *fptr_report;
            void init();
            bool loadData();
            void loadIimage();
            void loadDimage();
            void read(uint_32t_word, int);
            void write(uint_32t_word, int);
            int findIPTEPPN(uint_32t_word);
            int findDPTEPPN(uint_32t_word);
            int findITLBPPN(uint_32t_word);
            int findDTLBPPN(uint_32t_word);
            void updateITLBPPN(uint_32t_word, int);
            void updateDTLBPPN(uint_32t_word, int);
            int insertIMEM(uint_32t_word);
            int insertDMEM(uint_32t_word);
            void queryICACHE(uint_32t_word, int);
            void queryDCACHE(uint_32t_word, int);

            void dump();
            void report();
            uint_32t_word fetch();
            instruction decode(uint_32t_word);
            char getInstructionType(uint_32t_word);
            instruction parseRType(uint_32t_word);
            instruction parseIType(uint_32t_word);
            instruction parseJType(uint_32t_word);
            instruction parseSType(uint_32t_word);
            void execute(instruction);
            uint_32t_word signExtend16(uint_32t_word);
            uint_32t_word signExtend8(uint_32t_word);
            bool getSign(int);
            void checkWriteToRegZeroError(uint_32t_word);
            void _nor(instruction);
            void _nori(instruction);
            void _or(instruction);
            void _ori(instruction);
            void _xor(instruction);
            void _and(instruction);
            void _andi(instruction);
            void _nand(instruction);
            void _add(instruction);
            void _sub(instruction);
            void _sll(instruction);
            void _srl(instruction);
            void _sra(instruction);
            void _slt(instruction);
            void _slti(instruction);
            void _funct(instruction);
            void _lw(instruction);
            void _sw(instruction);
            void _lh(instruction);
            void _lhu(instruction);
            void _lb(instruction);
            void _lbu(instruction);
            void _sh(instruction);
            void _sb(instruction);
            void _lui(instruction);
            void _addi(instruction);
            void _beq(instruction);
            void _bne(instruction);
            void _j(instruction);
            void _jr(instruction);
            void _jal(instruction);
    };
} // namespace Simulator

#endif
