#pragma once
#include "cpu.h"


typedef struct instruction {

    void (*opcode_func)(Cpu* cpu);
    uint8_t cycles;

} Instruction;

extern Instruction opcodes[256];

void no_op(Cpu* cpu);

void load_a_immediate(Cpu *cpu);

void load_a_zp(Cpu *cpu);

void load_a_zp_x(Cpu *cpu);

void load_a_abs(Cpu *cpu);

void load_a_abs_x(Cpu *cpu);

void load_a_abs_y(Cpu *cpu);

void load_a_ind_x_pre(Cpu *cpu);

void load_a_ind_y_post(Cpu *cpu);










