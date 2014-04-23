LUA		= lua
MIPS_AS		= ../mips-assembler/mips-as.lua
MIPS_DATA	= ../mips-assembler/mips-data.lua
OFFSET		= 0x360

all		: iimage.bin dimage.bin
iimage.bin	: code.s
	$(LUA) $(MIPS_AS) code.s iimage.bin $(OFFSET)
dimage.bin	: data.txt
	$(LUA) $(MIPS_DATA) data.txt dimage.bin
clean		:
	-rm *.bin
