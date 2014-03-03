/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "const.h"
#include <cstdio>

extern WORD_32bit reg[REGISTER_SIZE];
extern WORD_32bit imemory[MEMORY_SIZE];
extern WORD_32bit dmemory[MEMORY_SIZE];
extern WORD_32bit cycle_counter;
extern WORD_32bit pc;

void init();
void load_basic_setting(FILE *iimage);
void load_imemory(FILE *iimage);
void load_dmemory(FILE *dimage);
WORD_32bit transformInitialPCtoSimulatorPC(WORD_32bit initPC);

#endif
