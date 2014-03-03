#ifndef INSTRUCTION_H
#define INSTRUCTION_H

/* 
 * My Structure instr for storing information of each 
 * instruction and it will be used in excution 
 * */
struct instruction {
    WORD_32bit opcode;
    WORD_32bit funct;
    WORD_32bit rs, rt, rd;
    WORD_32bit ci, cs, ca;
    instruction ();
};

#endif
