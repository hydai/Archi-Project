OBJS	= mips-as.o binary.o asm.o
CFLAGS	= -Wall -O0 -g

all	: mips-as

mips-as	: $(OBJS)
	$(CC) $(CFLAGS) -o $(.TARGET) $(.ALLSRC)

$(OBJS)	: $(.PREFIX).c
	$(CC) $(CFLAGS) -c $(.PREFIX).c

clean	:
	-rm *.o
	-rm mips-as

.PHONY	: all clean
