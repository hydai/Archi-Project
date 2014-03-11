/* binary.c - Functions to emit binary for the MIPS R3000 simulator  *
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "binary.h"

uint32_t rInst(uint8_t opcode,
               uint8_t rs,
               uint8_t rt,
               uint8_t rd,
               uint8_t shamt,
               uint8_t funct)
{
  uint32_t buf = 0xffffffff;
  uint32_t tmp;

  /* Check bounds of the arguments */
  assert( opcode < 0b111111 );
  assert( rs < 0b11111 );
  assert( rt < 0b11111 );
  assert( rd < 0b11111 );
  assert( shamt < 0b11111 );
  assert( funct < 0b111111 );
    
  /* Write bits into buffer */
  tmp = 0xffffffff;
  tmp &= opcode;
  tmp <<= 26;
  tmp |= 0x03ffffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= rs;
  tmp <<= 21;
  tmp |= 0xfc1fffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= rt;
  tmp <<= 16;
  tmp |= 0xffe0ffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= rd;
  tmp <<= 11;
  tmp |= 0xffff07ff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= shamt;
  tmp <<= 6;
  tmp |= 0xfffffc1f;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= funct;
  tmp <<= 0;
  tmp |= 0xffffffc0;
  buf &= tmp;

  return buf;
}

uint32_t iInst(uint8_t opcode,
               uint8_t rs,
               uint8_t rt,
               int16_t immediate)
{
  uint32_t buf = 0xffffffff;
  uint32_t tmp;

  /* Check bounds of the arguments */
  assert( opcode < 0b111111 );
  assert( rs < 0b11111 );
  assert( rt < 0b11111 );
    
  /* Write bits into buffer */
  tmp = 0xffffffff;
  tmp &= opcode;
  tmp <<= 26;
  tmp |= 0x03ffffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= rs;
  tmp <<= 21;
  tmp |= 0xfc1fffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= rt;
  tmp <<= 16;
  tmp |= 0xffe0ffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= immediate;
  tmp <<= 0;
  tmp |= 0xffff0000;
  buf &= tmp;

  return buf;
}

uint32_t jInst(uint8_t opcode,
               uint32_t address)
{
  uint32_t buf = 0xffffffff;
  uint32_t tmp;

  /* Check bounds of the arguments */
  assert( opcode < 0b111111 );
  assert( address < 0b11111111111111111111111111 );
    
  /* Write bits into buffer */
  tmp = 0xffffffff;
  tmp &= opcode;
  tmp <<= 26;
  tmp |= 0x03ffffff;
  buf &= tmp;

  tmp = 0xffffffff;
  tmp &= address;
  tmp <<= 0;
  tmp |= 0xfc000000;
  buf &= tmp;

  return buf;
}
