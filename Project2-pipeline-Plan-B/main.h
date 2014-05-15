/*
  main.h - Pipelined MIPS Simulator Header File

  Written in 2014 by Jack Force <jack1243star@gmail.com>

  To the extent possible under law, the author(s) have dedicated all
  copyright and related and neighboring rights to this software to the
  public domain worldwide. This software is distributed without any
  warranty.

  You should have received a copy of the CC0 Public Domain Dedication
  along with this software. If not, see
  <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

/* Number of registers */
#define REG_NUM   32
/* Memory size in bytes */
#define MEM_SIZE  1024
/* Word width in bytes */
#define WORD_SIZE 4

/* Internal representation of opcodes */
enum InstCode
{
  ADD,
  SUB,
  AND,
  OR,
  XOR,
  NOR,
  NAND,
  SLT,
  SLL,
  SRL,
  SRA,
  JR,
  ADDI,
  LW,
  LH,
  LHU,
  LB,
  LBU,
  SW,
  SH,
  SB,
  LUI,
  ANDI,
  ORI,
  NORI,
  SLTI,
  BEQ,
  BNE,
  J,
  JAL,
  HALT,
  NOP
};

/* Decoded instruction */
struct Inst
{
  enum InstCode instruction;
  uint8_t  rs, rt, rd;
  uint32_t c;
};

void fprintInstName(FILE* file_ptr, struct Inst* inst);

#endif
