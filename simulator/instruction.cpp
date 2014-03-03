#include "instruction.h"

instruction::instruction() {
    opcode = 0;
    funct = 0;
    rs = 0;
    rt = 0;
    rd = 0;
    ci = 0;
    cs = 0;
    ca = 0;
}
