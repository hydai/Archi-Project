#include "simulator.h"

namespace Simulator {
    Simulator::Simulator() {
        this->init();
        this->loadData();
    }
    Simulator::~Simulator() {
        fclose(errordump);
        fclose(snapshot);
    }
    void Simulator::init() {
        memset(imemory, 0, sizeof(imemory));
        memset(dmemory, 0, sizeof(dmemory));
        memset(reg, 0, sizeof(reg));
        pc = 0;
        cycleCounter = 0;
        runtimeStatus = STATUS_NORMAL;
    }
    bool Simulator::loadData() {
        // open files, if something goes wrong, return false
        iimage = fopen("iimage.bin", "r");
        dimage = fopen("dimage.bin", "r");
        errordump = fopen("error_dump.rpt", "w");
        snapshot = fopen("snapshot.rpt", "w");
        if (iimage == NULL || dimage == NULL) {
            printf("Cannot open iimage.bin or dimage.bin\n");
            return false;
        }
        
        this->loadIimage();
        this->loadDimage();
        return true;
    }
    void Simulator::loadIimage() {
        // load iimage
        // get pc
        fread(&pc, sizeof(uint_32t_word), 1, iimage);
        // get cycle number
        uint_32t_word cycleNumber;
        fread(&cycleNumber, sizeof(uint_32t_word), 1, iimage);
        // get instructions
        for (int i = 0; i < cycleNumber; i++) {
            fread((imemory+(pc/4)+i), sizeof(uint_32t_word), 1, iimage);
        }
        fclose(iimage);
    }
    void Simulator::loadDimage() {
        // get reg $sp
        fread(&reg[29], sizeof(uint_32t_word), 1, dimage);
        // get word number
        uint_32t_word wordNumber;
        fread(&wordNumber, sizeof(uint_32t_word), 1, dimage);
        // get data
        for (int i = 0; i < wordNumber; i++) {
            fread((dmemory+i), sizeof(uint_32t_word), 1, dimage);
        }
        fclose(dimage);
    }
    void Simulator::dump() {
        fprintf(snapshot, "cycle %u\n", cycleCounter++);
        for (int i = 0; i < REG_SIZE; i++) {
            fprintf(snapshot, "$%02d: 0x%08X\n", i, reg[i]);
        }
        fprintf(snapshot, "PC: 0x%08X\n", pc);
        fprintf(snapshot, "\n\n");
    }
    void Simulator::run() {
        while (true) {
            uint_32t_word currentInstruction = 0;
            instruction instr;
            // dump status
            this->dump();
            // fetch opcode
            currentInstruction = this->fetch();
            if (runtimeStatus == STATUS_HALT) {
                break;
            }
            // decode
            instr = this->decode(currentInstruction);
            // excution
            this->excute(instr);
            if (runtimeStatus == STATUS_HALT) {
                break;
            }
        }
    }
    uint_32t_word Simulator::fetch() {
        uint_32t_word opcode;
        bool hasError = false;

        // Check address overflow
        if (pc >= 1024 || pc < 0) {
            fprintf(errordump, "Address overflow in cycle: %d\n", cycleCounter);
            hasError = true;
        }
        // Check misalignment error
        if (pc % 4 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            hasError = true;
        }

        // check if the error happens or not
        if (hasError) {
            runtimeStatus = STATUS_HALT;
            return 0;
        }

        // get opcode
        opcode = imemory[pc/4];
        // PC move to next instruction
        pc += 4;
        return opcode;
    }

    char Simulator::getInstructionType(uint_32t_word code) {
        uint_32t_word opcode = code >> 26;
        char type;
        switch (opcode) {
            case op_funct:
                type = 'R';
                break;
            case op_addi:   case op_lw:
            case op_lh:     case op_lhu:
            case op_lb:     case op_lbu:
            case op_sw:     case op_sh:     
            case op_sb:     case op_lui:    
            case op_andi:   case op_ori:    
            case op_nori:   case op_slti:   
            case op_beq:    case op_bne:
                type = 'I';
                break;
            case op_j:      case op_jal:
                type = 'J';
                break;
            default:
                type = 'S';
                break;
        }
        return type;
    }

    instruction Simulator::decode(uint_32t_word code) {
        instruction instr;
        switch (getInstructionType(code)) {
            case 'R':
                instr = parseRType(code);
                break;
            case 'I':
                instr = parseIType(code);
                break;
            case 'J':
                instr = parseJType(code);
                break;
            default:
                // halt instruction
                instr = parseSType(code);
                break;
        }
        return instr;
    }

    instruction Simulator::parseRType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.rs = ((code << 6) >> 6) >> 21;
        instr.rt = ((code << 11) >> 11) >> 16;
        instr.rd = ((code << 16) >> 16) >> 11;
        instr.cs = ((code << 21) >> 21) >> 6;
        instr.funct = code & 0x0000003F;
        return instr;
    }
    instruction Simulator::parseIType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.rs = ((code << 6) >> 6) >> 21;
        instr.rt = ((code << 11) >> 11) >> 16;
        instr.ci = code & 0x0000FFFF;
        return instr;
    }
    instruction Simulator::parseJType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        instr.ca = code & 0x03FFFFFF;
        return instr;
    }
    instruction Simulator::parseSType(uint_32t_word code) {
        instruction instr;
        instr.op = code >> 26;
        return instr;
    }

    uint_32t_word Simulator::signExtend(uint_32t_word code) {
        if ((code >> 15)&1) {
            return code | 0xFFFF0000;
        }
        return code;
    }
    void Simulator::_lw(instruction instr) {
        int offset = (int) signExtend(instr.ci);

        // write to $0
        if (instr.rt == 0) {
            fprintf(errordump, "Write $0 error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_CONTINUE;
        }
        // Check address overflow
        if (reg[instr.rs] + offset >= 1024 || reg[instr.rs] + offset < 0) {
            fprintf(errordump, "Address overflow in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }
        // Check misalignment error
        if ((reg[instr.rs] + offset)%4 != 0) {
            fprintf(errordump, "Misalignment error in cycle: %d\n", cycleCounter);
            runtimeStatus = STATUS_HALT;
        }

        // check if the error happens or not
        if (runtimeStatus != STATUS_NORMAL) {
            return;
        }
        // TODO: Continue status
        reg[instr.rt] = dmemory[reg[instr.rs]+offset];
    }

    void Simulator::_nor(instruction instr) {
        reg[instr.rd] = ~(reg[instr.rs] | reg[instr.rt]);
    }
    void Simulator::_funct(instruction instr) {
        switch (instr.funct) {
            case funct_nor:
                _nor(instr);
                break;
            default:
                break;
        }
    }
    void Simulator::excute(instruction instr) {
        switch (instr.op) {
            case op_halt:
                runtimeStatus = STATUS_HALT;
                break;
            case op_lw:
                _lw(instr);
                break;
            case op_funct:
                _funct(instr);
                break;
            default:
                break;
        }
    }
} // namespace Simulator
