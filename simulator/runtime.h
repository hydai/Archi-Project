/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "const.h"


/* 
 * My Structure instr for storing information of each 
 * instruction and it will be used in excution 
 * */
struct instr {
    WORD_32bit opcode;
    WORD_32bit funct;
    WORD_32bit rs, rt, rd;
    WORD_32bit ci, cs, ca;
    instr ();
};

#endif
