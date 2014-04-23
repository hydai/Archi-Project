#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace Simulator {
    namespace CONSTANT {
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
    } // namespace CONSTANT

    struct instruction {
        CONSTANT::uint_32t_word op;
        CONSTANT::uint_32t_word funct;
        CONSTANT::uint_32t_word rs, rt, rd;
        CONSTANT::uint_32t_word ci, cs, ca;
    };

    class Simulator {
        public:
            /**
             * Constructor without parameter.
             * Initial status by default.
             */
            Simulator();
            /**
             * Destructor.
             * Return all allocate memory and reset data.
             */
            ~Simulator();
            /**
             * run:
             *      Start simulation.
             * Return:
             *      No return value.
             */
            void run();
        private:
            CONSTANT::uint_32t_word imemory[CONSTANT::MEMORY_SIZE/4];
            CONSTANT::uint_32t_word dmemory[CONSTANT::MEMORY_SIZE/4];
            CONSTANT::uint_32t_word reg[CONSTANT::REG_SIZE];
            CONSTANT::uint_32t_word pc;
            CONSTANT::uint_32t_word cycleCounter;
            CONSTANT::uint_32t_word runtimeStatus;
            FILE *iimage, *dimage, *errordump, *snapshot;
    };
} // namespace Simulator

#endif
