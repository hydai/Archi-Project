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
  /* Instruction register */
  uint32_t ir = 0;
  /* Flush flag */
  int flush;
  /* Stall flag */
  int stall;

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

    /* Reset stall and flush flags */
    stall = 0;
    flush = 0;

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
        fprintf(errordump, "Write $0 error in cycle %" PRIu32 "\n", cycle);
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
        fprintf(errordump, "Write $0 error in cycle %" PRIu32 "\n", cycle);
      break;

    case JAL:
      /* Load $31 with PC + 4 */
      reg[31] = pc + 4;
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
    /* TODO */

    /* Execute instruction */
    instEX = instID;
    /* TODO */

    /* Instruction decode */
    if(!stall)
      decode(&instID, ir);

    /* Instruction fetch */
    /* Load instruction register */
    ir = imemory[pc/4];
    /* Increment program counter */
    if(!stall && !flush)
      pc = pc + 4;

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
      fprintf(snapshot, " to_be_stalled");
    else if(/* Forward */0)
      ;/* TODO */
    else
      fprintf(snapshot, "\n");

    fprintf(snapshot, "EX: ");
    fprintInstName(snapshot, &instEX);
    /* Print forward information */
    if(/* Forward */0)
      ;/* TODO */
    else
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
