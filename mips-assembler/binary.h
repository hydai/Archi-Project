/* binary.h - Functions to emit binary for the MIPS R3000 simulator  *
 *                                                                   *
 * Written in 2014 by Jack Yang <jack1243star@gmail.com>             *
 *                                                                   *
 * To the extent possible under law, the author(s) have dedicated    *
 * all copyright and related and neighboring rights to this software *
 * to the public domain worldwide. This software is distributed      *
 * without any warranty.                                             *
 *                                                                   *
 * You should have received a copy of the CC0 Public Domain          *
 * Dedication along with this software. If not, see                  *
 * <http://creativecommons.org/publicdomain/zero/1.0/>.              */

#ifndef BINARY_H
#define BINARY_H

#include <stdint.h>

/* Return binary of an R-Type Instruction */
uint32_t rInst(uint8_t opcode,
               uint8_t rs,
               uint8_t rt,
               uint8_t rd,
               uint8_t shamt,
               uint8_t funct);

/* Return binary of an I-Type Instruction */
uint32_t iInst(uint8_t opcode,
               uint8_t rs,
               uint8_t rt,
               int16_t immediate);

/* Return binary of an J-Type Instruction */
uint32_t jInst(uint8_t opcode,
               uint32_t address);

#endif
