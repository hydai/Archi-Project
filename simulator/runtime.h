/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/

#ifndef RUNTIME_H
#define RUNTIME_H

#include "const.h"
#include "instruction.h"
#include "parse.h"
#include "execution.h"

instruction parseInstr(WORD_32bit instr);
WORD_32bit executeInstr(instruction tmpInstr);
#endif
