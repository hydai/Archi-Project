/*
Simulator, NTHU, CS, Archi, Project1
Copyright (C) 2014 Hung-Ying, Dai
All rights reserved.
*/

/*
 * This is part of simulater. It defined units and constants
 * basically.
 * */
#ifndef CONST_H_
#define CONST_H_

/* Define basic unit */
#define MEMORY_SIZE 256
#define REGISTER_SIZE 32
#define WORD_32bit unsigned int

/* Alias of registers */
#define sp 29

/* Mapping the op name to hex code */
#define op_funct   0x00
#define op_addi    0x08
#define op_lw      0x23
#define op_lh      0x21
#define op_lhu     0x25
#define op_lb      0x20
#define op_lbu     0x24
#define op_sw      0x2B
#define op_sh      0x29
#define op_sb      0x28
#define op_lui     0x0F
#define op_andi    0x0C
#define op_ori     0x0D
#define op_nori    0x0E
#define op_slti    0x0A
#define op_beq     0x04
#define op_bne     0x05
#define op_j       0x02
#define op_jal     0x03
#define op_halt    0x3F

/* Mapping the op name to hex code */
#define funct_add   0x20
#define funct_sub   0x22
#define funct_and   0x24
#define funct_or    0x25
#define funct_xor   0x26
#define funct_nor   0x27
#define funct_nand  0x28
#define funct_slt   0x2A
#define funct_sll   0x00
#define funct_srl   0x02
#define funct_sra   0x03
#define funct_jr    0x08

/* Mapping the runtime status to hex code*/
#define status_halt 0x3F
#endif  // CONST_H_
