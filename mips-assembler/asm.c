/* asm.c - Functions to read MIPS R3000 assembly                     *
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "asm.h"

void strip(char* str)
{
  int i = 0, j = 0;

  /* Skip leading whitespace */
  while(!isgraph(str[i]) && str[i] != '\0')
    i++;
  /* Clean empty string with only whitespace and comment */
  if(str[i] == '\0' || str[i] == '#')
  {
    str[0] = '\0';
    return;
  }
  /* Strip comment */
  while(str[j] != '\0' && str[j] != '#')
    j++;
  /* Strip whitespace before comment */
  while(!isgraph(str[j-1]) && j-i >= 0)
    j--;
  /* Fill in new string */
  memmove(str, str + i, (j-i)*sizeof(char));
  /* Close new string */
  str[j-i] = '\0';

  return;
}

char* stripLabel(char* str)
{
  int i, j;
  char* label = NULL;

  for(i = 0; str[i] != '\0'; i++)
  {
    /* Found label */
    if(str[i] == ':')
    {
      /* Make space for found label */
      label = (char*)malloc((i+1)*sizeof(char));
      /* Copy label name */
      memcpy(label, str, i*sizeof(char));
      /* Close the label name string */
      label[i] = '\0';
      /* Skip the colon */
      i++;
      /* Skip leading whitespace */
      while(!isgraph(str[i]) && str[i] != '\0')
        i++;
      /* Strip label part from original string */
      for(j = 0; str[j+i] != '\0'; j++)
        str[j] = str[j+i];
      /* Close the original string */
      str[j] = '\0';
    }
  }

  return label;
}
