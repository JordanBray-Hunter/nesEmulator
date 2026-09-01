#pragma once
#include "cpu.h"

// could seperate all instructions into a type file. eg access transfer
// branch, arithmetic

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

// Sub with carry


void sub_with_carry_immediate(Cpu *cpu);

void sub_with_carry_zp(Cpu *cpu);

void sub_with_carry_zp_x(Cpu *cpu);

void sub_with_carry_abs(Cpu *cpu);

void sub_with_carry_abs_x(Cpu *cpu);

void sub_with_carry_abs_y(Cpu *cpu);

void sub_with_carry_ind_x_pre(Cpu *cpu);

void sub_with_carry_ind_y_post(Cpu *cpu);

// Stack

void push_a_to_stack(Cpu *cpu);

void pull_a_from_stack(Cpu *cpu);

void push_p_to_stack(Cpu *cpu);

void pull_p_from_stack(Cpu *cpu);

// Jump

void jump_abs(Cpu *cpu);

void jump_ind(Cpu *cpu);

void jump_to_sub(Cpu *cpu);

void return_from_sub(Cpu *cpu);


//Compare A

void compare_a_immediate(Cpu *cpu);

void compare_a_zp(Cpu *cpu);

void compare_a_zp_x(Cpu *cpu);

void compare_a_abs(Cpu *cpu);

void compare_a_abs_x(Cpu *cpu);

void compare_a_abs_y(Cpu *cpu);

void compare_a_ind_x_pre(Cpu *cpu);

void compare_a_ind_y_post(Cpu *cpu);

//Compare X

void compare_x_immediate(Cpu *cpu);

void compare_x_zp(Cpu *cpu);

void compare_x_abs(Cpu *cpu);

//Compare Y

void compare_y_immediate(Cpu *cpu);

void compare_y_zp(Cpu *cpu);

void compare_y_abs(Cpu *cpu);


// Branch

void branch_carry_clear(Cpu * cpu);
void branch_carry_set(Cpu * cpu);

void branch_equal(Cpu * cpu);
void branch_not_equal(Cpu * cpu);

void branch_plus(Cpu * cpu);
void branch_minus(Cpu * cpu);

void branch_overflow_clear(Cpu * cpu);
void branch_overflow_set(Cpu * cpu);




