#pragma once
#include "cpu.h"

typedef struct instruction
{

    void (*opcode_func)(Cpu *cpu);
    uint8_t cycles;

} Instruction;

extern Instruction opcodes[256];

void no_op(Cpu *cpu);

// Load A

void load_a_immediate(Cpu *cpu);

void load_a_zp(Cpu *cpu);

void load_a_zp_x(Cpu *cpu);

void load_a_abs(Cpu *cpu);

void load_a_abs_x(Cpu *cpu);

void load_a_abs_y(Cpu *cpu);

void load_a_ind_x_pre(Cpu *cpu);

void load_a_ind_y_post(Cpu *cpu);

// Load X
void load_x_immediate(Cpu *cpu);

void load_x_zp(Cpu *cpu);

void load_x_zp_y(Cpu *cpu);

void load_x_abs(Cpu *cpu);

void load_x_abs_y(Cpu *cpu);

// Load Y
void load_y_immediate(Cpu *cpu);

void load_y_zp(Cpu *cpu);

void load_y_zp_x(Cpu *cpu);

void load_y_abs(Cpu *cpu);

void load_y_abs_x(Cpu *cpu);

// Store A in memory
void store_a_zp(Cpu *cpu);

void store_a_zp_x(Cpu *cpu);

void store_a_abs(Cpu *cpu);

void store_a_abs_x(Cpu *cpu);

void store_a_abs_y(Cpu *cpu);

void store_a_ind_x_pre(Cpu *cpu);

void store_a_ind_y_post(Cpu *cpu);

// Transfer

void transfer_a_x(Cpu *cpu);

void transfer_x_a(Cpu *cpu);

void transfer_a_y(Cpu *cpu);

void transfer_y_a(Cpu *cpu);

void transfer_x_sp(Cpu *cpu);

void transfer_sp_x(Cpu *cpu);

// Flags

void clear_carry(Cpu *cpu);

void clear_interrupt_disable(Cpu *cpu);

void clear_overflow(Cpu *cpu);

void clear_decimal(Cpu *cpu);

void set_decimal(Cpu *cpu);

void set_interrupt_disable(Cpu *cpu);

void set_carry(Cpu *cpu);

// //Add with carry A=A + Memory + C
void add_with_carry_immediate(Cpu *cpu);

void add_with_carry_zp(Cpu *cpu);

void add_with_carry_zp_x(Cpu *cpu);

void add_with_carry_abs(Cpu *cpu);

void add_with_carry_abs_x(Cpu *cpu);

void add_with_carry_abs_y(Cpu *cpu);

void add_with_carry_ind_x_pre(Cpu *cpu);

void add_with_carry_ind_y_post(Cpu *cpu);
