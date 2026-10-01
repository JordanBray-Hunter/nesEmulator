#pragma once
#include "cpu.h"

// could seperate all instructions into a type file. eg access transfer
// branch, arithmetic

typedef struct instruction
{
    void (*opcode_func)(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
    uint16_t (*addr_func)(Cpu *, bool);
    uint8_t cycles;

} Instruction;

extern Instruction opcodes[256];

// NOP
void no_op(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Load
void load_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void load_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void load_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Store
void store_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void store_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void store_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Arithmetic
void add_with_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void sub_with_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Transfer
void transfer_a_to_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void transfer_x_to_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void transfer_a_to_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void transfer_y_to_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void transfer_x_to_sp(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void transfer_sp_to_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Flags
void clear_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void clear_interrupt_disable(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void clear_overflow(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void clear_decimal(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void set_decimal(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void set_interrupt_disable(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void set_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Stack
void push_a_to_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void pull_a_from_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void push_p_to_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void pull_p_from_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Jump / Interrupt
void jump(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void jump_to_sub(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void return_from_sub(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void break_irq(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void return_from_interrupt(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Compare
void compare_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void compare_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void compare_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Branch
void branch_carry_clear(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_carry_set(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_equal(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_not_equal(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_plus(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_minus(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_overflow_clear(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void branch_overflow_set(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Bitwise
void bitwise_and_immediate(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void bitwise_or_immediate(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void bitwise_xor(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Bit Test
void bit_test_zp(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Increment / Decrement
void increment_memory(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void increment_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void increment_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

void decrement_memory(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void decrement_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void decrement_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Shifts
void shift_left_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void shift_left(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void shift_right_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void shift_right(Cpu *cpu, uint16_t (*func)(Cpu *, bool));

// Rotates
void rotate_left_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void rotate_left(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void rotate_right_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool));
void rotate_right(Cpu *cpu, uint16_t (*func)(Cpu *, bool));