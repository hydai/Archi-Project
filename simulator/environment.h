/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/
#include "const.h"

#ifndef ENVIRONMENT_H
#define ENVIRONMENT

WORD_32bit reg[REGISTER_SIZE];
WORD_32bit imemory[MEMORY_SIZE];
WORD_32bit dmemory[MEMORY_SIZE];
WORD_32bit cycle_counter;
WORD_32bit pc;

void init();
void load_basic_setting(FILE *iimage);
void load_imemory(FILE *iimage);
void load_dmemory(FILE *dimage);
WORD_32bit transformInitialPCtoSimulatorPC(WORD_32bit initPC);

#endif
