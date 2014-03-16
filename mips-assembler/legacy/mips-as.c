/* mips-as.c - MIPS R3000 assembler                                  *
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
#include <stdint.h>
#include <assert.h>
#include "asm.h"
#include "binary.h"

#define MAX_LINE_LENGTH 1024

int main(int argc, char* argv[])
{
  char buf[MAX_LINE_LENGTH];
  char* label = NULL;
  FILE* input = NULL;
  FILE* output = NULL;

  /* The offset from base address */
  int offset = 0;
  /* Line number */
  int line = 0;

  /* Prepare I/O files */
  if(argc == 2)
  {
    /* Default output file name */
    input = fopen(argv[1], "r");
    output = fopen("output.bin", "wb");
  }
  else if(argc == 3)
  {
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
  }
  else
  {
    /* No file name provided */
    printf("Usage: test input-file [output-file]\n");
    return EXIT_SUCCESS;
  }

  /* Make sure files exist */
  assert(input != NULL);
  assert(output != NULL);

  /* Process each line */
  while(fgets(buf, sizeof(buf), input) != NULL)
  {
    /* Increment input line number */
    line++;
    /* Strip line of spaces and comments */
    strip(buf);
    /* Skip blank line */
    if(buf[0] == '\0')
      continue;

    /* Check for label */
    if((label = stripLabel(buf)) != NULL)
    {
      if(buf[0] == '\0')
      {
        /* Lone label */
        fprintf(stderr, "Error: Label \"%s\" cannot be resolved to an address.\n", label);
        fprintf(stderr, "\tat %s:%d\n", argv[1], line);
        exit(EXIT_FAILURE);
      }
    }
    printf("----\nStripped:\t\"%s\"\n", buf); /* DEBUG */
    if(label != NULL)
      printf("Label:\t\t\"%s\"\n", label); /* DEBUG */
    else
      printf("Label:\t\tNone\n");
    /* TODO */

    /* Increment instruction offset */
    offset++;
  }

  fclose(input);
  fclose(output);
  return EXIT_SUCCESS;
}
