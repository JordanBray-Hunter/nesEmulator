#pragma once
#include "cpu.h"


typedef struct instruction {

    void (*opcode_func)(Cpu* cpu);

} Instruction;


extern Instruction opcodes[256] = {


};








