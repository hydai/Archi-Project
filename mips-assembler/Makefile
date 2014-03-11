OBJS		= mips-as.o binary.o asm.o
CFLAGS		= -Wall -O0 -g

all		: mips-as

mips-as		: $(OBJS)
	$(CC) $(CFLAGS) -o mips-as $(OBJS)

mips-as.o	: mips-as.c
	$(CC) $(CFLAGS) -c mips-as.c

binary.o	: binary.c
	$(CC) $(CFLAGS) -c binary.c

asm.o		: asm.c
	$(CC) $(CFLAGS) -c asm.c

clean		:
	-rm *.o
	-rm mips-as

.PHONY		: all clean
