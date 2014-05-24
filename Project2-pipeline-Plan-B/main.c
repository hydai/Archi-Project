/*
  main.c - Pipelined MIPS Simulator Main Program

  Written in 2014 by Jack Force <jack1243star@gmail.com>

  To the extent possible under law, the author(s) have dedicated all
  copyright and related and neighboring rights to this software to the
  public domain worldwide. This software is distributed without any
  warranty.

  You should have received a copy of the CC0 Public Domain Dedication
  along with this software. If not, see
  <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#include "main.h"

int main(int argc, char* argv[])
{
  /* Instruction memory image file */
  FILE* iimage;
  /* Data memory image file */
  FILE* dimage;
  /* Per-cycle state output file */
  FILE* snapshot;
  /* Error message output file */
  FILE* errordump;

  /* Register content */
  uint32_t reg[REG_NUM];
  /* Instruction memory */
  uint32_t imemory[MEM_SIZE/WORD_SIZE];
  /* Data memory */
  uint32_t dmemory[MEM_SIZE/WORD_SIZE];
  /* Size of memory content */
  uint32_t inputsize;
  /* Program counter */
  uint32_t pc;
  /* Program counter before modify */
  uint32_t oldpc;
  /* Instruction register */
  uint32_t ir = 0;
  /* Instruction register stalled */
  uint32_t irstalled = 0;
  /* Flush flag */
  int flush = 0;
  /* Flushed flag */
  int flushed = 0;
  /* Stall flag */
  int stall = 0;
  /* Stalled flag */
  int stalled = 0;
  /* ID forward from EX-DM for rs flag */
  int fwdIDfromEXDMrs;
  /* ID forward from EX-DM for rt flag */
  int fwdIDfromEXDMrt;
  /* EX forward from EX-DM for rs flag */
  int fwdEXfromEXDMrs;
  /* EX forward from EX-DM for rt flag */
  int fwdEXfromEXDMrt;
  /* EX forward from DM-WB for rs flag */
  int fwdEXfromDMWBrs;
  /* EX forward from DM-WB for rt flag */
  int fwdEXfromDMWBrt;
  /* Write to $0 error flag */
  int writeToZeroError;
  /* Number overflow error flag */
  int numOverflowError;
  /* Memory address overflow error flag */
  int addrOverflowError;
  /* Address misaligned */
  int alignError;
  /* Temporary values for operands */
  uint32_t op0, op1;

  /* Write back stage instruction */
  struct Inst instWB;
  /* Write back stage data bus */
  uint32_t dataWB = 0;

  /* Data memory access stage instruction */
  struct Inst instDM;
  /* Data memory access stage data bus */
  uint32_t dataDM = 0;

  /* Execute instruction stage instruction */
  struct Inst instEX;
  /* Execute instruction stage data bus */
  uint32_t dataEX = 0;

  /* Instruction decode stage instruction */
  struct Inst instID;
  /* Instruction decode stage data bus */
  uint32_t dataID = 0;
  /* Instruction decode stage stalled instruction */
  struct Inst instIDstalled;

  /* Loop counter */
  int i;
  /* Quit flag */
  int quit = 0;
  /* Current cycle */
  uint32_t cycle = 0;

  /* Open input and output files */
  iimage = fopen("iimage.bin", "rb");
  assert(iimage != NULL);
  dimage = fopen("dimage.bin", "rb");
  assert(dimage != NULL);
  errordump = fopen("error_dump.rpt", "w");
  snapshot = fopen("snapshot.rpt", "w");

  /* Initialize each register with 0 */
  memset(reg, 0, sizeof(uint32_t)*REG_NUM);

  /* Load instruction memory image */
  /* Initialize to zero */
  memset(imemory, 0, MEM_SIZE);
  /* Load program counter value */
  fread(&pc, sizeof(uint32_t), 1, iimage);
  /* Get number of words to read */
  fread(&inputsize, sizeof(uint32_t), 1, iimage);
  /* Load memory content */
  for(i = 0; i < inputsize; i++)
    fread(imemory+(pc/4)+i, sizeof(uint32_t), 1, iimage);
  fclose(iimage);

  /* Load data memory image */
  /* Initialize to zero */
  memset(dmemory, 0, MEM_SIZE);
  /* Load stack pointer value */
  fread(&reg[29], sizeof(uint32_t), 1, dimage);
  /* Get number of words to read */
  fread(&inputsize, sizeof(uint32_t), 1, dimage);
  /* Load memory content */
  for(i = 0; i < inputsize; i++)
    fread(dmemory+i, sizeof(uint32_t), 1, dimage);
  fclose(dimage);

  /* Initialize each stage with NOP */
  instWB.instruction = NOP;
  instDM.instruction = NOP;
  instEX.instruction = NOP;
  instID.instruction = NOP;

  /* Execute */
  while(!quit)
  {
    /* Print cycle index */
    fprintf(snapshot, "cycle %" PRIu32 "\n", cycle);
    /* Dump register contents */
    for(i = 0; i < REG_NUM; i++)
      fprintf(snapshot, "$%02d: 0x%08" PRIX32 "\n", i, reg[i]);
    /* Print program counter */
    fprintf(snapshot, "PC: 0x%08" PRIX32 "\n", pc);

    /* Reset flags */
    stalled = stall ? 1 : 0;
    stall = 0;
    flushed = flush ? 1 : 0;
    flush = 0;
    fwdIDfromEXDMrs = 0;
    fwdIDfromEXDMrt = 0;
    fwdEXfromEXDMrs = 0;
    fwdEXfromEXDMrt = 0;
    fwdEXfromDMWBrs = 0;
    fwdEXfromDMWBrt = 0;
    writeToZeroError  = 0;
    numOverflowError  = 0;
    addrOverflowError = 0;
    alignError        = 0;

    /* Write back */
    instWB = instDM;
    dataWB = dataDM;
    switch (instWB.instruction)
    {
    case ADD:
    case SUB:
    case AND:
    case OR:
    case XOR:
    case NOR:
    case NAND:
    case SLT:
    case SLL:
    case SRL:
    case SRA:
      /* Load result to rd */
      if(instWB.rd != 0)
        reg[instWB.rd] = dataWB;
      else
        writeToZeroError = 1;
      break;

    case ADDI:
    case LW:
    case LH:
    case LHU:
    case LB:
    case LBU:
    case LUI:
    case ANDI:
    case ORI:
    case NORI:
    case SLTI:
      /* Load result to rt */
      if(instWB.rt != 0)
        reg[instWB.rt] = dataWB;
      else
        writeToZeroError = 1;
      break;

    case JAL:
      /* Load $31 with PC + 4 */
      reg[31] = dataWB;
      break;

    case HALT:
      /* Halt */
      quit = 1;
      break;

    default:
      break;
    }

    /* Data memory access */
    instDM = instEX;
    dataDM = dataEX;
    switch(instDM.instruction)
    {
    case LW:
    case LH:
    case LHU:
    case LB:
    case LBU:
    case SW:
    case SH:
    case SB:
      /* Check for address overflow */
      if(dataDM >= 1024)
      {
        /* Address overflow */
        addrOverflowError = 1;
      }
      /* Do actual operation */
      switch(instDM.instruction)
      {
      case LW:
        if(dataDM % 4 != 0)
        {
          /* Misalignment error */
          alignError = 1;
        }
        if(!addrOverflowError && !alignError)
        {
          /* Do actual load */
          dataDM = dmemory[dataDM/4];
        }
        break;

      case LH:
        if(dataDM % 2 != 0)
        {
          /* Misalignment error */
          alignError = 1;
        }
        if(!addrOverflowError && !alignError)
        {
          /* Do actual load, shift if not aligned to word width */
          if(dataDM % 4 != 0)
            dataDM = dmemory[dataDM/4] >> 16;
          else
            dataDM = dmemory[dataDM/4];
          /* Sign extend to 32-bit */
          dataDM = sext16(dataDM & 0x0000FFFF);
        }
        break;

      case LHU:
        if(dataDM % 2 != 0)
        {
          /* Misalignment error */
          alignError = 1;
        }
        if(!addrOverflowError && !alignError)
        {
          /* Do actual load, shift if not aligned to word width */
          if(dataDM % 4 != 0)
            dataDM = dmemory[dataDM/4] >> 16;
          else
            dataDM = dmemory[dataDM/4];
          /* Clear out higher half word */
          dataDM = dataDM & 0x0000FFFF;
        }
        break;

      case LB:
        if(!addrOverflowError)
        {
          /* Determine shift amount of byte to load */
          switch(dataDM % 4)
          {
          case 0:
            dataDM = dmemory[dataDM/4] >> 0;
            break;
          case 1:
            dataDM = dmemory[dataDM/4] >> 8;
            break;
          case 2:
            dataDM = dmemory[dataDM/4] >> 16;
            break;
          case 3:
            dataDM = dmemory[dataDM/4] >> 24;
            break;
          }
          /* Load and sign extend the value */
          dataDM = sext8(dataDM & 0x000000FF);
        }
        break;

      case LBU:
        if(!addrOverflowError)
        {
          /* Determine shift amount of byte to load */
          switch(dataDM % 4)
          {
          case 0:
            dataDM = dmemory[dataDM/4] >> 0;
            break;
          case 1:
            dataDM = dmemory[dataDM/4] >> 8;
            break;
          case 2:
            dataDM = dmemory[dataDM/4] >> 16;
            break;
          case 3:
            dataDM = dmemory[dataDM/4] >> 24;
            break;
          }
          /* Mask the value to 8-bits */
          dataDM = dataDM & 0x000000FF;
        }
        break;

      case SW:
        if(dataDM % 4 != 0)
        {
          /* Misalignment error */
          alignError = 1;
        }
        if(!addrOverflowError && !alignError)
        {
          /* Do actual store */
          dmemory[dataDM/4] = reg[instDM.rt];
        }
        break;

      case SH:
        if(dataDM % 2 != 0)
        {
          /* Misalignment error */
          alignError = 1;
        }
        if(!addrOverflowError && !alignError)
        {
          /* Determine which half of word to store */
          if(dataDM % 4 != 0)
            dmemory[dataDM/4] =
              (dmemory[dataDM/4] & 0x0000FFFF) |
              ((reg[instDM.rt] << 16) & 0xFFFF0000);
          else
            dmemory[dataDM/4] =
              (dmemory[dataDM/4] & 0xFFFF0000) |
              ((reg[instDM.rt] << 0) & 0x0000FFFF);
        }
        break;

      case SB:
        if(!addrOverflowError && !alignError)
        {
          /* Determine which byte of word to store */
          switch(dataDM % 4)
          {
          case 0:
            dmemory[dataDM/4] =
              (dmemory[dataDM/4] & 0xFFFFFF00) |
              ((reg[instDM.rt] << 0) & 0x000000FF);
            break;
          case 1:
            dmemory[dataDM/4] =
              (dmemory[dataDM/4] & 0xFFFF00FF) |
              ((reg[instDM.rt] << 8) & 0x0000FF00);
            break;
          case 2:
            dmemory[dataDM/4] =
              (dmemory[dataDM/4] & 0xFF00FFFF) |
              ((reg[instDM.rt] << 16) & 0x00FF0000);
            break;
          case 3:
            dmemory[dataDM/4] =
              (dmemory[dataDM/4] & 0x00FFFFFF) |
              ((reg[instDM.rt] << 24) & 0xFF000000);
            break;
          }
        }
        break;

      default:
        assert(0);
        break;
      }
      break;

    default:
      break;
    }

    /* Execute instruction */
    if(stalled)
    {
      instEX.instruction = NOP;
    }
    else
    {
      instEX = instID;
      dataEX = dataID;
    }
    /* Forward and execute control */
    switch(instEX.instruction)
    {
      /* rd depends on rs and rt */
    case ADD:
    case SUB:
    case AND:
    case OR:
    case XOR:
    case NOR:
    case NAND:
    case SLT:
      if(isWriteToRdInst(instDM.instruction))
      {
        if(instEX.rs == instDM.rd && instEX.rs != 0)
        {
          /* Forward rs from EX-DM */
          fwdEXfromEXDMrs = 1;
        }
        if(instEX.rt == instDM.rd && instEX.rt != 0)
        {
          /* Forward rt from EX-DM */
          fwdEXfromEXDMrt = 1;
        }
      }
      if(isWriteToRtInst(instDM.instruction))
      {
        if(instEX.rs == instDM.rt && instEX.rs != 0)
        {
          /* Forward rs from EX-DM */
          fwdEXfromEXDMrs = 1;
        }
        if(instEX.rt == instDM.rt && instEX.rt != 0)
        {
          /* Forward rt from EX-DM */
          fwdEXfromEXDMrt = 1;
        }
      }
      if(isWriteToRdInst(instWB.instruction))
      {
        if(instEX.rs == instWB.rd && instEX.rs != 0)
        {
          /* Forward rs from DM-WB */
          fwdEXfromDMWBrs = 1;
        }
        if(instEX.rt == instWB.rd && instEX.rt != 0)
        {
          /* Forward rt from DM-WB */
          fwdEXfromDMWBrt = 1;
        }
      }
      if(isWriteToRtInst(instWB.instruction))
      {
        if(instEX.rs == instWB.rt && instEX.rs != 0)
        {
          /* Forward rs from DM-WB */
          fwdEXfromDMWBrs = 1;
        }
        if(instEX.rt == instWB.rt && instEX.rt != 0)
        {
          /* Forward rt from DM-WB */
          fwdEXfromDMWBrt = 1;
        }
      }
      op0 =
        fwdEXfromEXDMrs ? dataDM :
        fwdEXfromDMWBrs ? dataWB : reg[instEX.rs];
      op1 =
        fwdEXfromEXDMrt ? dataDM :
        fwdEXfromDMWBrt ? dataWB : reg[instEX.rt];
      /* Do actual operation */
      switch(instEX.instruction)
      {
      case ADD:
        dataEX = op0 + op1;
        if(getSign(op0) == getSign(op1) &&
           getSign(op0) != getSign(dataEX))
        {
          /* Overflow */
          numOverflowError = 1;
        }
        break;
      case SUB:
        dataEX = op0 - op1;
        if((op1 == 0x80000000) ||
           (getSign(op0) == getSign(-op1) &&
            getSign(op0) != getSign(dataEX)))
        {
          /* Overflow */
          numOverflowError = 1;
        }
        break;
      case AND:
        dataEX = op0 & op1;
        break;
      case OR:
        dataEX = op0 | op1;
        break;
      case XOR:
        dataEX = op0 ^ op1;
        break;
      case NOR:
        dataEX = ~(op0 | op1);
        break;
      case NAND:
        dataEX = ~(op0 & op1);
        break;
      case SLT:
        dataEX = (int)op0 < (int)op1 ? 1 : 0;
        break;

      default:
        assert(0);
        break;
      }
      break;

      /* rd depends on rt only */
    case SLL:
    case SRL:
    case SRA:
      if(instEX.rt != 0)
      {
        if(isWriteToRdInst(instDM.instruction))
        {
          if(instEX.rt == instDM.rd)
          {
            /* Forward rt from EX-DM */
            fwdEXfromEXDMrt = 1;
          }
        }
        if(isWriteToRtInst(instDM.instruction))
        {
          if(instEX.rt == instDM.rt)
          {
          /* Forward rt from EX-DM */
          fwdEXfromEXDMrt = 1;
          }
        }
        if(isWriteToRdInst(instWB.instruction))
        {
          if(instEX.rt == instWB.rd)
          {
            /* Forward rt from DM-WB */
            fwdEXfromDMWBrt = 1;
          }
        }
        if(isWriteToRtInst(instWB.instruction))
        {
          if(instEX.rt == instWB.rt)
          {
          /* Forward rt from DM-WB */
            fwdEXfromDMWBrt = 1;
          }
        }
      }
      op0 =
        fwdEXfromEXDMrt ? dataDM :
        fwdEXfromDMWBrt ? dataWB : reg[instEX.rt];
      op1 = instEX.c;
      /* Do actual operation */
      switch(instEX.instruction)
      {
      case SLL:
        dataEX = op0 << op1;
        break;
      case SRL:
        dataEX = op0 >> op1;
        break;
      case SRA:
        dataEX = op0;
        for(i = 0; i < instEX.c; i++)
        {
          dataEX = dataEX >> 1;
          dataEX = dataEX | (getSign(op0) ? 0x80000000 : 0);
        }
        break;

      default:
        assert(0);
        break;
      }
      break;

      /* rt depends on rs only */
    case ADDI:
    case ANDI:
    case ORI:
    case NORI:
    case SLTI:
      if(instEX.rs != 0)
      {
        if(isWriteToRdInst(instDM.instruction))
        {
          if(instEX.rs == instDM.rd)
          {
            /* Forward rs from EX-DM */
            fwdEXfromEXDMrs = 1;
          }
        }
        if(isWriteToRtInst(instDM.instruction))
        {
          if(instEX.rs == instDM.rt)
          {
            /* Forward rs from EX-DM */
            fwdEXfromEXDMrs = 1;
          }
        }
        if(isWriteToRdInst(instWB.instruction))
        {
          if(instEX.rs == instWB.rd)
          {
          /* Forward rs from DM-WB */
            fwdEXfromDMWBrs = 1;
          }
        }
        if(isWriteToRtInst(instWB.instruction))
        {
          if(instEX.rs == instWB.rt)
          {
            /* Forward rs from DM-WB */
            fwdEXfromDMWBrs = 1;
          }
        }
      }
      op0 =
        fwdEXfromEXDMrs ? dataDM :
        fwdEXfromDMWBrs ? dataWB : reg[instEX.rs];
      op1 = instEX.c;
      /* Do actual operation */
      switch(instEX.instruction)
      {
      case ADDI:
	op1 = sext16(op1);
        dataEX = op0 + op1;
        if(getSign(op0) == getSign(op1) &&
           getSign(op0) != getSign(dataEX))
        {
          /* Overflow */
          numOverflowError = 1;
        }
        break;
      case ANDI:
        dataEX = op0 & op1;
        break;
      case ORI:
        dataEX = op0 | op1;
        break;
      case NORI:
        dataEX = ~(op0 | op1);
        break;
      case SLTI:
	op1 = sext16(op1);
        dataEX = (int)op0 < (int)op1 ? 1 : 0;
        break;

      default:
        assert(0);
        break;
      }
      break;

      /* rt depends on rs, calculate address */
    case LW:
    case LH:
    case LHU:
    case LB:
    case LBU:
      /* Check forwarding condition */
      if(instEX.rs != 0)
      {
        if(isWriteToRdInst(instDM.instruction))
        {
          if(instEX.rs == instDM.rd)
          {
            /* Forward rs from EX-DM */
            fwdEXfromEXDMrs = 1;
          }
        }
        if(isWriteToRtInst(instDM.instruction))
        {
          if(instEX.rs == instDM.rt)
          {
            /* Forward rs from EX-DM */
            fwdEXfromEXDMrs = 1;
          }
        }
        if(isWriteToRdInst(instWB.instruction))
        {
          if(instEX.rs == instWB.rd)
          {
            /* Forward rs from DM-WB */
          fwdEXfromDMWBrs = 1;
          }
        }
        if(isWriteToRtInst(instWB.instruction))
        {
          if(instEX.rs == instWB.rt)
          {
            /* Forward rs from DM-WB */
            fwdEXfromDMWBrs = 1;
          }
        }
      }
      /* Check for number overflow */
      if((getSign(sext16(instDM.c)) ==
          getSign(reg[instDM.rs])) &&
         (getSign(sext16(instDM.c)) !=
          getSign(sext16(instDM.c) + reg[instDM.rs])))
      {
        /* Number overflow */
        numOverflowError = 1;
      }
      /* Get the operands */
      op0 = sext16(instEX.c);
      op1 =
        fwdEXfromEXDMrs ? dataDM :
        fwdEXfromDMWBrs ? dataWB : reg[instEX.rs];
      /* Pass down the address */
      dataEX = op0 + op1;
      break;

      /* memory depends on rs and rt, calculate address */
    case SW:
    case SH:
    case SB:
      /* Check forwarding condition */
      if(isWriteToRdInst(instDM.instruction))
      {
        if(instEX.rs == instDM.rd && instEX.rs != 0)
        {
          /* Forward rs from EX-DM */
          fwdEXfromEXDMrs = 1;
        }
        if(instEX.rt == instDM.rd && instEX.rt != 0)
        {
          /* Forward rt from EX-DM */
          fwdEXfromEXDMrt = 1;
        }
      }
      if(isWriteToRtInst(instDM.instruction))
      {
        if(instEX.rs == instDM.rt && instEX.rs != 0)
        {
          /* Forward rs from EX-DM */
          fwdEXfromEXDMrs = 1;
        }
        if(instEX.rt == instDM.rt && instEX.rt != 0)
        {
          /* Forward rt from EX-DM */
          fwdEXfromEXDMrt = 1;
        }
      }
      if(isWriteToRdInst(instWB.instruction))
      {
        if(instEX.rs == instWB.rd && instEX.rs != 0)
        {
          /* Forward rs from DM-WB */
          fwdEXfromDMWBrs = 1;
        }
        if(instEX.rt == instWB.rd && instEX.rt != 0)
        {
          /* Forward rt from DM-WB */
          fwdEXfromDMWBrt = 1;
        }
      }
      if(isWriteToRtInst(instWB.instruction))
      {
        if(instEX.rs == instWB.rt && instEX.rs != 0)
        {
          /* Forward rs from DM-WB */
          fwdEXfromDMWBrs = 1;
        }
        if(instEX.rt == instWB.rt && instEX.rt != 0)
        {
          /* Forward rt from DM-WB */
          fwdEXfromDMWBrt = 1;
        }
      }
      /* Check for number overflow */
      if((getSign(sext16(instDM.c)) ==
          getSign(reg[instDM.rs])) &&
         (getSign(sext16(instDM.c)) !=
          getSign(sext16(instDM.c) + reg[instDM.rs])))
      {
        /* Number overflow */
        numOverflowError = 1;
      }
      /* Get the operands */
      op0 = sext16(instEX.c);
      op1 =
        fwdEXfromEXDMrs ? dataDM :
        fwdEXfromDMWBrs ? dataWB : reg[instEX.rs];
      /* Pass down the address */
      dataEX = op0 + op1;
      break;

    default:
      break;
    }

    /* Instruction decode */
    if(stalled)
      instID = instIDstalled;
    else if(flushed)
      instID.instruction = NOP;
    else
      decode(&instID, ir);
    /* Evaluate instructions with stall and flush */
    switch(instID.instruction)
    {
      /* rd depends on rs and rt */
    case ADD:
    case SUB:
    case AND:
    case OR:
    case XOR:
    case NOR:
    case NAND:
    case SLT:
      if(isReadFromMemInst(instEX.instruction))
      {
        if(instID.rs == instEX.rt && instID.rs != 0)
          stall = 1;
        if(instID.rt == instEX.rt && instID.rt != 0)
          stall = 1;
      }
      break;

      /* rd depends on rt only */
    case SLL:
    case SRL:
    case SRA:
      if(isReadFromMemInst(instEX.instruction))
      {
        if(instID.rt == instEX.rt && instID.rt != 0)
          stall = 1;
      }
      break;

      /* rt depends on rs only */
    case ADDI:
    case ANDI:
    case ORI:
    case NORI:
    case SLTI:
      if(isReadFromMemInst(instEX.instruction))
      {
        if(instID.rs == instEX.rt && instID.rs != 0)
          stall = 1;
      }
      break;

      /* Branch instructions */
    case BEQ:
    case BNE:
      if((isWriteToRdInst(instEX.instruction) &&
          ((instID.rs == instEX.rd && instID.rs != 0) ||
	   (instID.rt == instEX.rd && instID.rt != 0))) ||
         (isWriteToRtInst(instEX.instruction) &&
          ((instID.rs == instEX.rt && instID.rs != 0) ||
	   (instID.rt == instEX.rt && instID.rt != 0))) ||
         (isReadFromMemInst(instEX.instruction) &&
          ((instID.rs == instEX.rt && instID.rs != 0) ||
	   (instID.rt == instEX.rt && instID.rt != 0))) ||
         (isReadFromMemInst(instDM.instruction) &&
          ((instID.rs == instDM.rt && instID.rs != 0) ||
	   (instID.rt == instDM.rt && instID.rt != 0))))
      {
        /* Stall the datapath */
        stall = 1;
      }
      else
      {
        /* Check for forward conditions */
        if(isWriteToRdInst(instDM.instruction) ||
           isWriteToRtInst(instDM.instruction))
        {
          if(instID.rs != 0 &&
	     (instID.rs == instDM.rd || instID.rs == instDM.rt))
          {
            /* Forward rs from EX-DM */
            fwdIDfromEXDMrs = 1;
          }
          if(instID.rt != 0 &&
	     (instID.rt == instDM.rd || instID.rt == instDM.rt))
          {
            /* Forward rt from EX-DM */
            fwdIDfromEXDMrt = 1;
          }
        }
        op0 = fwdIDfromEXDMrs ? dataEX : reg[instID.rs];
        op1 = fwdIDfromEXDMrt ? dataEX : reg[instID.rt];
        /* Branch */
        if(instID.instruction == BEQ)
        {
          /* Branch on equal */
          if(op0 == op1)
          {
            pc = pc + 4 * sext16(instID.c);
            flush = 1;
          }
        }
        else
        {
          /* Branch on not equal */
          if(op0 != op1)
          {
            pc = pc + 4 * sext16(instID.c);
            flush = 1;
          }
        }
      }
      break;

      /* Jump instructions */
    case JR:
      /* Check for stall condition */
      if((isWriteToRdInst(instEX.instruction) &&
          (instID.rs == instEX.rd && instID.rs != 0)) ||
         (isWriteToRtInst(instEX.instruction) &&
          (instID.rs == instEX.rt && instID.rs != 0)) ||
         (isReadFromMemInst(instEX.instruction) &&
          (instID.rs == instEX.rt && instID.rs != 0)) ||
         (isReadFromMemInst(instDM.instruction) &&
          (instID.rs == instDM.rt && instID.rs != 0)))
      {
        /* Stall the datapath */
        stall = 1;
      }
      else
      {
        /* Check for forward conditions */
        if(isWriteToRdInst(instDM.instruction) ||
           isWriteToRtInst(instDM.instruction))
        {
          if(instID.rs != 0 &&
	     (instID.rs == instDM.rd || instID.rs == instDM.rt))
          {
            /* Forward rs from EX-DM */
            fwdIDfromEXDMrs = 1;
          }
        }
        /* Write to program counter */
        if(fwdIDfromEXDMrs)
          pc = dataEX;
        else
          pc = reg[instID.rs];
        /* Flush the instruction register */
        flush = 1;
      }
      break;

    case J:
      /* Do the jump */
      pc = (pc & 0xF0000000) | ((instID.c << 2) & 0x0FFFFFFC);
      /* Flush the instruction register */
      flush = 1;
      break;

    case JAL:
      /* Pass program counter down the data path */
      dataID = pc;
      /* Do the jump */
      pc = (pc & 0xF0000000) | ((instID.c << 2) & 0x0FFFFFFC);
      /* Flush the instruction register */
      flush = 1;
      break;

    default:
      break;
    }
    if(stall)
    {
      /* Stall instruction */
      instIDstalled = instID;
    }

    /* Instruction fetch */
    /* Load instruction register */
    if(stalled)
      ir = irstalled;
    else if(flush)
      ir = imemory[oldpc/4];
    else
      ir = imemory[pc/4];
    /* Increment program counter */
    if(stall)
      irstalled = ir;
    else if(!flush)
      pc = pc + 4;
    /* Save pc in case of branching */
    oldpc = pc;

    /* Print each stage */
    /* Print instruction register */
    fprintf(snapshot, "IF: 0x%08" PRIX32, ir);
    /* Print stall/flush information */
    if(stall)
      fprintf(snapshot, " to_be_stalled\n");
    else if(flush)
      fprintf(snapshot, " to_be_flushed\n");
    else
      fprintf(snapshot, "\n");

    fprintf(snapshot, "ID: ");
    fprintInstName(snapshot, &instID);
    /* Print stall/forward information */
    if(stall)
      fprintf(snapshot, " to_be_stalled\n");
    else
    {
      if(fwdIDfromEXDMrs)
        fprintf(snapshot, " fwd_EX-DM_rs_$%" PRIu8, instID.rs);
      if(fwdIDfromEXDMrt)
        fprintf(snapshot, " fwd_EX-DM_rt_$%" PRIu8, instID.rt);
      fprintf(snapshot, "\n");
    }

    fprintf(snapshot, "EX: ");
    fprintInstName(snapshot, &instEX);
    /* Print forward information */
    if(fwdEXfromEXDMrs)
      fprintf(snapshot, " fwd_EX-DM_rs_$%" PRIu8, instEX.rs);
    else if(fwdEXfromDMWBrs)
      fprintf(snapshot, " fwd_DM-WB_rs_$%" PRIu8, instEX.rs);
    if(fwdEXfromEXDMrt)
      fprintf(snapshot, " fwd_EX-DM_rt_$%" PRIu8, instEX.rt);
    else if(fwdEXfromDMWBrt)
      fprintf(snapshot, " fwd_DM-WB_rt_$%" PRIu8, instEX.rt);
    fprintf(snapshot, "\n");

    fprintf(snapshot, "DM: ");
    fprintInstName(snapshot, &instDM);
    fprintf(snapshot, "\n");

    fprintf(snapshot, "WB: ");
    fprintInstName(snapshot, &instWB);
    fprintf(snapshot, "\n");

    /* Make space for next entry */
    fprintf(snapshot, "\n\n");

    /* Increment cycle */
    cycle = cycle + 1;

    /* Report error */
    if(writeToZeroError)
      fprintf(errordump, "Write $0 error in cycle: %" PRIu32 "\n", cycle);
    if(addrOverflowError)
      fprintf(errordump, "Address overflow in cycle: %" PRIu32 "\n", cycle);
    if(alignError)
      fprintf(errordump, "Misalignment error in cycle: %" PRIu32 "\n", cycle);
    if(numOverflowError)
      fprintf(errordump, "Number overflow in cycle: %" PRIu32 "\n", cycle);

    /* Halt if address overflow or misalignment error occured */
    if(addrOverflowError || alignError)
      quit = 1;
  }

  fclose(snapshot);
  fclose(errordump);
  return EXIT_SUCCESS;
}

/* Decode instruction and store it */
void decode(struct Inst* inst, uint32_t ir)
{
  switch((ir>>26)&0x3F)
  {
    /* R-Type instructions */
  case 0x00:
    /* Check for NOP instruction */
    if(ir == 0x00000000)
    {
      /* The instruction is NOP */
      inst->instruction = NOP;
    }
    else
    {
      /* Determine actual instruction */
      switch(ir & 0x3F)
      {
      case 0x20: /* add */
        inst->instruction = ADD;
        break;
      case 0x22: /* sub */
        inst->instruction = SUB;
        break;
      case 0x24: /* and */
        inst->instruction = AND;
        break;
      case 0x25: /* or */
        inst->instruction = OR;
        break;
      case 0x26: /* xor */
        inst->instruction = XOR;
        break;
      case 0x27: /* nor */
        inst->instruction = NOR;
        break;
      case 0x28: /* nand */
        inst->instruction = NAND;
        break;
      case 0x2A: /* slt */
        inst->instruction = SLT;
        break;
      case 0x00: /* sll */
        inst->instruction = SLL;
        break;
      case 0x02: /* srl */
        inst->instruction = SRL;
        break;
      case 0x03: /* sra */
        inst->instruction = SRA;
        break;
      case 0x08: /* jr */
        inst->instruction = JR;
        break;
      default:
        assert(0);
        break;
      }
      /* Load operands */
      inst->rs = (ir>>21) & 0x1F;
      inst->rt = (ir>>16) & 0x1F;
      inst->rd = (ir>>11) & 0x1F;
      inst->c  = (ir>>6)  & 0x1F;
    }
    break;

    /* I-Type instructions */
  case 0x08: /* addi */
    inst->instruction = ADDI;
    goto iinst_load_operand;
  case 0x23: /* lw */
    inst->instruction = LW;
    goto iinst_load_operand;
  case 0x21: /* lh */
    inst->instruction = LH;
    goto iinst_load_operand;
  case 0x25: /* lhu */
    inst->instruction = LHU;
    goto iinst_load_operand;
  case 0x20: /* lb */
    inst->instruction = LB;
    goto iinst_load_operand;
  case 0x24: /* lbu */
    inst->instruction = LBU;
    goto iinst_load_operand;
  case 0x2B: /* sw */
    inst->instruction = SW;
    goto iinst_load_operand;
  case 0x29: /* sh */
    inst->instruction = SH;
    goto iinst_load_operand;
  case 0x28: /* sb */
    inst->instruction = SB;
    goto iinst_load_operand;
  case 0x0F: /* lui */
    inst->instruction = LUI;
    goto iinst_load_operand;
  case 0x0C: /* andi */
    inst->instruction = ANDI;
    goto iinst_load_operand;
  case 0x0D: /* ori */
    inst->instruction = ORI;
    goto iinst_load_operand;
  case 0x0E: /* nori */
    inst->instruction = NORI;
    goto iinst_load_operand;
  case 0x0A: /* slti */
    inst->instruction = SLTI;
    goto iinst_load_operand;
  case 0x04: /* beq */
    inst->instruction = BEQ;
    goto iinst_load_operand;
  case 0x05: /* bne */
    inst->instruction = BNE;
    goto iinst_load_operand;
  iinst_load_operand:
    /* Load operands */
    inst->rs = (ir>>21) & 0x1F;
    inst->rt = (ir>>16) & 0x1F;
    inst->c  = ir & 0xFFFF;
    break;

    /* J-Type instructions */
  case 0x02: /* j */
    inst->instruction = J;
    goto jinst_load_operand;
  case 0x03: /* jal */
    inst->instruction = JAL;
    goto jinst_load_operand;
  jinst_load_operand:
    /* Load operand */
    inst->c = ir & 0x03FFFFFF;
    break;

    /* Specialized instruction */
  case 0x3F: /* halt */
    inst->instruction = HALT;
    break;

  default:
    assert(0);
    break;
  }
}

/* Print the mnemonic of the instruction to a file using fprintf() */
void fprintInstName(FILE* file_ptr, struct Inst* inst)
{
  switch(inst->instruction)
  {
  case ADD:
    fprintf(file_ptr, "ADD");
    break;
  case SUB:
    fprintf(file_ptr, "SUB");
    break;
  case AND:
    fprintf(file_ptr, "AND");
    break;
  case OR:
    fprintf(file_ptr, "OR");
    break;
  case XOR:
    fprintf(file_ptr, "XOR");
    break;
  case NOR:
    fprintf(file_ptr, "NOR");
    break;
  case NAND:
    fprintf(file_ptr, "NAND");
    break;
  case SLT:
    fprintf(file_ptr, "SLT");
    break;
  case SLL:
    fprintf(file_ptr, "SLL");
    break;
  case SRL:
    fprintf(file_ptr, "SRL");
    break;
  case SRA:
    fprintf(file_ptr, "SRA");
    break;
  case JR:
    fprintf(file_ptr, "JR");
    break;
  case ADDI:
    fprintf(file_ptr, "ADDI");
    break;
  case LW:
    fprintf(file_ptr, "LW");
    break;
  case LH:
    fprintf(file_ptr, "LH");
    break;
  case LHU:
    fprintf(file_ptr, "LHU");
    break;
  case LB:
    fprintf(file_ptr, "LB");
    break;
  case LBU:
    fprintf(file_ptr, "LBU");
    break;
  case SW:
    fprintf(file_ptr, "SW");
    break;
  case SH:
    fprintf(file_ptr, "SH");
    break;
  case SB:
    fprintf(file_ptr, "SB");
    break;
  case LUI:
    fprintf(file_ptr, "LUI");
    break;
  case ANDI:
    fprintf(file_ptr, "ANDI");
    break;
  case ORI:
    fprintf(file_ptr, "ORI");
    break;
  case NORI:
    fprintf(file_ptr, "NORI");
    break;
  case SLTI:
    fprintf(file_ptr, "SLTI");
    break;
  case BEQ:
    fprintf(file_ptr, "BEQ");
    break;
  case BNE:
    fprintf(file_ptr, "BNE");
    break;
  case J:
    fprintf(file_ptr, "J");
    break;
  case JAL:
    fprintf(file_ptr, "JAL");
    break;
  case HALT:
    fprintf(file_ptr, "HALT");
    break;
  case NOP:
    fprintf(file_ptr, "NOP");
    break;
  default:
    assert(0);
    break;
  }

  return;
}

/* Returns true if the instruction writes to rd */
int isWriteToRdInst(enum InstCode code)
{
  switch(code)
  {
  case ADD:
  case SUB:
  case AND:
  case OR:
  case XOR:
  case NOR:
  case NAND:
  case SLT:
  case SLL:
  case SRL:
  case SRA:
    return 1;
  default:
    return 0;
  }
}

/* Returns true if the instruction writes to rt */
int isWriteToRtInst(enum InstCode code)
{
  switch(code)
  {
  case ADDI:
  case LW:
  case LH:
  case LHU:
  case LB:
  case LBU:
  case LUI:
  case ANDI:
  case ORI:
  case NORI:
  case SLTI:
    return 1;
  default:
    return 0;
  }
}

/* Returns true if the instruction writes to memory */
int isWriteToMemInst(enum InstCode code)
{
  switch(code)
  {
  case SW:
  case SH:
  case SB:
    return 1;
  default:
    return 0;
  }
}

/* Returns true if the instruction reads from memory */
int isReadFromMemInst(enum InstCode code)
{
  switch(code)
  {
  case LW:
  case LH:
  case LHU:
  case LB:
  case LBU:
    return 1;
  default:
    return 0;
  }
}

/* Sign extends a 16-bit integer to 32-bit */
uint32_t sext16(uint32_t x)
{
  if((x >> 15) & 0x1)
  {
    return x | 0xFFFF0000;
  }
  else
  {
    return x & 0x0000FFFF;
  }
}

/* Sign extends a 8-bit integer to 32-bit */
uint32_t sext8(uint32_t x)
{
  if((x >> 7) & 0x1)
  {
    return x | 0xFFFFFF00;
  }
  else
  {
    return x & 0x000000FF;
  }
}

/* Return sign bit of a 32-bit integer */
int getSign(uint32_t x)
{
  if((x >> 31) & 0x1)
    return 1;
  else
    return 0;
}
