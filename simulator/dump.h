/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/

#ifndef DUMP_H_
#define DUMP_H_
#include "const.h"
#include "environment.h"
#include <cstdio>



WORD_32bit printPC(WORD_32bit simuPC);
void memory_dump(FILE *snapshot);

#endif  // DUMP_H_
