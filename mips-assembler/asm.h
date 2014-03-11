/* asm.h - Functions to read MIPS R3000 assembly                     *
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

#ifndef ASM_H
#define ASM_H

/* 
 * Strip str of surrounding whitespace and comment.
 */
void strip(char* str);

/*
 * If str contains a label, the label part is removed from str, and the
 * return value is a new string containing the label name. If no label
 * is found, return NULL.
 */
char* stripLabel(char* str);

#endif
