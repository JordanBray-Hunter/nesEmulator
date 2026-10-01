#include "opcodes.h"
#include "cpu.h"
#include "addressing_modes.h"
#include <stdio.h>

#define UNUSED_ADDR (void)func

void no_op(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    return;
}

void load_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    cpu->a = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->a);
    return;
}

// Load X
void load_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    cpu->x = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->x);
    return;
}

// Load Y (fixed name; was accidentally a duplicate load_x)
void load_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    cpu->y = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->y);
    return;
}

// Store A

void store_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    bus_write(cpu->bus, func(cpu, false), cpu->a);
}

// Store X

void store_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    bus_write(cpu->bus, func(cpu, false), cpu->x);
}

// Store Y

void store_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    bus_write(cpu->bus, func(cpu, false), cpu->y);
}

// Add with carry

void add_with_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint8_t carry_in = (cpu->p & C_BIT) ? 1 : 0;
    uint16_t address = func(cpu, true); // was uint8_t
    uint8_t value = bus_read(cpu->bus, address);
    uint16_t result = cpu->a + value + carry_in;
    update_alu_flags(cpu, cpu->a, value, result);
    cpu->a = (uint8_t)result;
}

void sub_with_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint8_t carry_in = (cpu->p & C_BIT) ? 1 : 0;
    uint16_t address = func(cpu, true); // was uint8_t
    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);
    uint16_t result = cpu->a + value + carry_in;
    update_alu_flags(cpu, cpu->a, value, result);
    cpu->a = (uint8_t)result;
}

// Transfer

void transfer_a_to_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->x = cpu->a;
    update_zn_flags(cpu, cpu->x);
}

void transfer_x_to_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->a = cpu->x;
    update_zn_flags(cpu, cpu->a);
}

void transfer_a_to_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->y = cpu->a;
    update_zn_flags(cpu, cpu->y);
}

void transfer_y_to_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->a = cpu->y;
    update_zn_flags(cpu, cpu->a);
}

void transfer_x_to_sp(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->sp = cpu->x;
}

void transfer_sp_to_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->x = cpu->sp;
    update_zn_flags(cpu, cpu->x);
}

// Flags (implied)
void clear_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p &= ~(C_BIT);
}
void clear_interrupt_disable(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p &= ~(I_DISABLE_BIT);
}
void clear_overflow(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p &= ~(V_BIT);
}
void clear_decimal(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p &= ~(D_BIT);
}
void set_decimal(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p |= D_BIT;
}
void set_interrupt_disable(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p |= I_DISABLE_BIT;
}
void set_carry(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p |= C_BIT;
}

// Stack
void push_a_to_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    bus_write(cpu->bus, (0x0100 + cpu->sp), cpu->a);
    cpu->sp--;
}

void pull_a_from_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->sp++;
    cpu->a = bus_read(cpu->bus, 0x0100 + cpu->sp);
    update_zn_flags(cpu, cpu->a);
}

void push_p_to_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    bus_write(cpu->bus, (0x0100 + cpu->sp), (cpu->p) | (1 << 5) | (1 << 4));
    cpu->sp--;
}

void pull_p_from_stack(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->sp++;
    uint8_t flags = bus_read(cpu->bus, 0x0100 + cpu->sp);
    flags = (flags & ~B_BIT) | U_BIT;
    cpu->p = flags;
}

// Jump

void jump(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    cpu->pc = func(cpu, false);
}

void jump_to_sub(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{

    bus_write(cpu->bus, (0x0100 + cpu->sp), ((cpu->pc + 1) & 0xFF00) >> 8);
    cpu->sp--;
    bus_write(cpu->bus, (0x0100 + cpu->sp), ((cpu->pc + 1) & 0x00FF));
    cpu->sp--;
    cpu->pc = func(cpu, false);
}

void return_from_sub(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->sp++;
    uint8_t low = bus_read(cpu->bus, 0x0100 + cpu->sp);
    cpu->sp++;
    uint8_t high = bus_read(cpu->bus, 0x0100 + cpu->sp);
    cpu->pc = ((high << 8) | low) + 1;
}

void break_irq(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    uint8_t high = (((cpu->pc + 1) & 0xFF00) >> 8);
    uint8_t low = (((cpu->pc + 1) & 0x00FF));
    bus_write(cpu->bus, (0x0100 + cpu->sp), high);
    cpu->sp--;
    bus_write(cpu->bus, (0x0100 + cpu->sp), low);
    cpu->sp--;
    bus_write(cpu->bus, (0x0100 + cpu->sp), (cpu->p | B_BIT | U_BIT));
    cpu->sp--;

    cpu->p |= I_DISABLE_BIT;

    uint8_t new_low = bus_read(cpu->bus, 0xFFFE);
    uint8_t new_high = bus_read(cpu->bus, 0xFFFF);

    cpu->pc = ((new_high << 8) | new_low);
}

void return_from_interrupt(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->sp++;
    uint8_t flags = bus_read(cpu->bus, 0x0100 + cpu->sp);
    flags = (flags & ~B_BIT) | U_BIT;
    cpu->p = flags;

    cpu->sp++;
    uint8_t low = bus_read(cpu->bus, 0x0100 + cpu->sp);
    cpu->sp++;
    uint8_t high = bus_read(cpu->bus, 0x0100 + cpu->sp);

    cpu->pc = ((high << 8) | low);
}

// Compare A
void compare_a(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

// Compare X

void compare_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->x, value);
}

// Compare Y

void compare_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->y, value);
}

// Branch

void branch_carry_clear(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & C_BIT) == 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}

void branch_carry_set(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & C_BIT) != 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}

void branch_equal(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & Z_BIT) != 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}
void branch_not_equal(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & Z_BIT) == 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}

void branch_plus(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & N_BIT) == 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}
void branch_minus(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & N_BIT) != 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}

void branch_overflow_clear(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & V_BIT) == 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}
void branch_overflow_set(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    int8_t offset = (int8_t)bus_read(cpu->bus, cpu->pc++);
    if ((cpu->p & V_BIT) != 0)
    {
        cpu->cycles_remaining++;
        uint16_t newvalue = cpu->pc + offset;
        if ((cpu->pc & 0xFF00) != (newvalue & 0xFF00))
        {
            cpu->cycles_remaining++;
        }
        cpu->pc = newvalue;
    }
}

// And

void bitwise_and(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    cpu->a &= value;
    update_zn_flags(cpu, cpu->a);
}

// Or
void bitwise_or(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    cpu->a |= value;
    update_zn_flags(cpu, cpu->a);
}

// Exlusive Or
void bitwise_xor(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    cpu->a ^= value;
    update_zn_flags(cpu, cpu->a);
}
// Bit Test

void bit_test(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    cpu->p &= ~(N_BIT | V_BIT | Z_BIT);
    cpu->p |= (value & (V_BIT | N_BIT));
    value &= cpu->a;
    cpu->p |= (value == 0 ? Z_BIT : 0);
}

// Increment

void increment_memory(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, false);

    uint8_t value = bus_read(cpu->bus, address);
    // Dummy Write
    bus_write(cpu->bus, address, value);
    value++;
    bus_write(cpu->bus, address, value);
    update_zn_flags(cpu, value);
}

void increment_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->x += 1;
    update_zn_flags(cpu, cpu->x);
}
void increment_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->y += 1;
    update_zn_flags(cpu, cpu->y);
}

// Decrement

void decrement_memory(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, false);

    uint8_t value = bus_read(cpu->bus, address);
    // Dummy Write
    bus_write(cpu->bus, address, value);
    value--;
    bus_write(cpu->bus, address, value);
    update_zn_flags(cpu, value);
}

void decrement_x(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->x -= 1;
    update_zn_flags(cpu, cpu->x);
}
void decrement_y(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->y -= 1;
    update_zn_flags(cpu, cpu->y);
}

// Arithmetic Shift Left

void shift_left_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p &= ~C_BIT;
    cpu->p |= (cpu->a & 0x80) ? C_BIT : 0;
    cpu->a <<= 1;
    update_zn_flags(cpu, cpu->a);
}
void shift_left(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, false);
    uint8_t value = bus_read(cpu->bus, address);

    bus_write(cpu->bus, address, value);

    cpu->p &= ~C_BIT;
    cpu->p |= (value & 0x80) ? C_BIT : 0;

    value <<= 1;
    bus_write(cpu->bus, address, value);
    update_zn_flags(cpu, value);
}
// Logical Shift Right

void shift_right_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    cpu->p &= ~C_BIT;
    cpu->p |= (cpu->a & 0x01) ? C_BIT : 0;
    cpu->a >>= 1;
    update_zn_flags(cpu, cpu->a);
}
void shift_right(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint16_t address = func(cpu, false);
    uint8_t value = bus_read(cpu->bus, address);

    bus_write(cpu->bus, address, value);

    cpu->p &= ~C_BIT;
    cpu->p |= (value & 0x01) ? C_BIT : 0;

    value >>= 1;
    bus_write(cpu->bus, address, value);
    update_zn_flags(cpu, value);
}
// Rotate Left

void rotate_left_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    uint8_t carry_in = (cpu->p & C_BIT ? 1 : 0);
    cpu->p &= ~C_BIT;
    cpu->p |= (cpu->a & 0x80) ? C_BIT : 0;
    cpu->a <<= 1;
    cpu->a |= carry_in;
    update_zn_flags(cpu, cpu->a);
}
void rotate_left(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint8_t carry_in = (cpu->p & C_BIT ? 1 : 0);
    uint16_t address = func(cpu, false);
    uint8_t value = bus_read(cpu->bus, address);

    bus_write(cpu->bus, address, value);

    cpu->p &= ~C_BIT;
    cpu->p |= (value & 0x80) ? C_BIT : 0;

    value <<= 1;
    value |= carry_in;

    bus_write(cpu->bus, address, value);
    update_zn_flags(cpu, value);
}

//  Rotate Right

void rotate_right_acc(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    UNUSED_ADDR;
    uint8_t carry_in = (cpu->p & C_BIT ? 1 : 0);
    cpu->p &= ~C_BIT;
    cpu->p |= (cpu->a & 0x01) ? C_BIT : 0;
    cpu->a >>= 1;
    cpu->a |= (carry_in << 7);

    update_zn_flags(cpu, cpu->a);
}
void rotate_right(Cpu *cpu, uint16_t (*func)(Cpu *, bool))
{
    uint8_t carry_in = (cpu->p & C_BIT ? 1 : 0);
    uint16_t address = func(cpu, false);
    uint8_t value = bus_read(cpu->bus, address);

    bus_write(cpu->bus, address, value);

    cpu->p &= ~C_BIT;
    cpu->p |= (value & 0x01) ? C_BIT : 0;

    value >>= 1;
    value |= (carry_in << 7);
    bus_write(cpu->bus, address, value);
    update_zn_flags(cpu, value);
}

Instruction opcodes[256] = {

    // NOP
    [0xEA] = {&no_op, &addr_unused, 2},

    // LDA
    [0xA9] = {&load_a, &addr_immediate, 2},
    [0xA5] = {&load_a, &addr_zp, 3},
    [0xB5] = {&load_a, &addr_zp_x, 4},
    [0xAD] = {&load_a, &addr_abs, 4},
    [0xBD] = {&load_a, &addr_abs_x, 4},
    [0xB9] = {&load_a, &addr_abs_y, 4},
    [0xA1] = {&load_a, &addr_ind_x_pre, 6},
    [0xB1] = {&load_a, &addr_ind_y_post, 5},

    // STA
    [0x85] = {&store_a, &addr_zp, 3},
    [0x95] = {&store_a, &addr_zp_x, 4},
    [0x8D] = {&store_a, &addr_abs, 4},
    [0x9D] = {&store_a, &addr_abs_x, 5},
    [0x99] = {&store_a, &addr_abs_y, 5},
    [0x81] = {&store_a, &addr_ind_x_pre, 6},
    [0x91] = {&store_a, &addr_ind_y_post, 6},

    // LDX
    [0xA2] = {&load_x, &addr_immediate, 2},
    [0xA6] = {&load_x, &addr_zp, 3},
    [0xB6] = {&load_x, &addr_zp_y, 4},
    [0xAE] = {&load_x, &addr_abs, 4},
    [0xBE] = {&load_x, &addr_abs_y, 4},

    // STX
    [0x86] = {&store_x, &addr_zp, 3},
    [0x96] = {&store_x, &addr_zp_y, 4},
    [0x8E] = {&store_x, &addr_abs, 4},

    // LDY
    [0xA0] = {&load_y, &addr_immediate, 2},
    [0xA4] = {&load_y, &addr_zp, 3},
    [0xB4] = {&load_y, &addr_zp_x, 4},
    [0xAC] = {&load_y, &addr_abs, 4},
    [0xBC] = {&load_y, &addr_abs_x, 4},

    // STY
    [0x84] = {&store_y, &addr_zp, 3},
    [0x94] = {&store_y, &addr_zp_x, 4},
    [0x8C] = {&store_y, &addr_abs, 4},

    // ADC
    [0x69] = {&add_with_carry, &addr_immediate, 2},
    [0x65] = {&add_with_carry, &addr_zp, 3},
    [0x75] = {&add_with_carry, &addr_zp_x, 4},
    [0x6D] = {&add_with_carry, &addr_abs, 4},
    [0x7D] = {&add_with_carry, &addr_abs_x, 4},
    [0x79] = {&add_with_carry, &addr_abs_y, 4},
    [0x61] = {&add_with_carry, &addr_ind_x_pre, 6},
    [0x71] = {&add_with_carry, &addr_ind_y_post, 5},

    // SBC
    [0xE9] = {&sub_with_carry, &addr_immediate, 2},
    [0xE5] = {&sub_with_carry, &addr_zp, 3},
    [0xF5] = {&sub_with_carry, &addr_zp_x, 4},
    [0xED] = {&sub_with_carry, &addr_abs, 4},
    [0xFD] = {&sub_with_carry, &addr_abs_x, 4},
    [0xF9] = {&sub_with_carry, &addr_abs_y, 4},
    [0xE1] = {&sub_with_carry, &addr_ind_x_pre, 6},
    [0xF1] = {&sub_with_carry, &addr_ind_y_post, 5},

    // Transfer
    [0xAA] = {&transfer_a_to_x, &addr_unused, 2},
    [0xA8] = {&transfer_a_to_y, &addr_unused, 2},
    [0xBA] = {&transfer_sp_to_x, &addr_unused, 2},
    [0x8A] = {&transfer_x_to_a, &addr_unused, 2},
    [0x9A] = {&transfer_x_to_sp, &addr_unused, 2},
    [0x98] = {&transfer_y_to_a, &addr_unused, 2},

    // Flags
    [0x18] = {&clear_carry, &addr_unused, 2},
    [0x38] = {&set_carry, &addr_unused, 2},
    [0x58] = {&clear_interrupt_disable, &addr_unused, 2},
    [0x78] = {&set_interrupt_disable, &addr_unused, 2},
    [0xD8] = {&clear_decimal, &addr_unused, 2},
    [0xF8] = {&set_decimal, &addr_unused, 2},
    [0xB8] = {&clear_overflow, &addr_unused, 2},

    // Stack
    [0x48] = {&push_a_to_stack, &addr_unused, 3},
    [0x68] = {&pull_a_from_stack, &addr_unused, 4},
    [0x08] = {&push_p_to_stack, &addr_unused, 3},
    [0x28] = {&pull_p_from_stack, &addr_unused, 4},

    // Jump
    [0x4C] = {&jump, &addr_abs, 3},
    [0x6C] = {&jump, &addr_indirect, 5},
    [0x20] = {&jump_to_sub, &addr_abs, 6},
    [0x60] = {&return_from_sub, &addr_unused, 6},
    [0x00] = {&break_irq, &addr_unused, 7},
    [0x40] = {&return_from_interrupt, &addr_unused, 6},

    // Compare A
    [0xC9] = {&compare_a, &addr_immediate, 2},
    [0xC5] = {&compare_a, &addr_zp, 3},
    [0xD5] = {&compare_a, &addr_zp_x, 4},
    [0xCD] = {&compare_a, &addr_abs, 4},
    [0xDD] = {&compare_a, &addr_abs_x, 4},
    [0xD9] = {&compare_a, &addr_abs_y, 4},
    [0xC1] = {&compare_a, &addr_ind_x_pre, 6},
    [0xD1] = {&compare_a, &addr_ind_y_post, 5},

    // Compare X
    [0xE0] = {&compare_x, &addr_immediate, 2},
    [0xE4] = {&compare_x, &addr_zp, 3},
    [0xEC] = {&compare_x, &addr_abs, 4},

    // Compare Y
    [0xC0] = {&compare_y, &addr_immediate, 2},
    [0xC4] = {&compare_y, &addr_zp, 3},
    [0xCC] = {&compare_y, &addr_abs, 4},

    // Branch
    [0x90] = {&branch_carry_clear, &addr_unused, 2},
    [0xB0] = {&branch_carry_set, &addr_unused, 2},
    [0xF0] = {&branch_equal, &addr_unused, 2},
    [0xD0] = {&branch_not_equal, &addr_unused, 2},
    [0x10] = {&branch_plus, &addr_unused, 2},
    [0x30] = {&branch_minus, &addr_unused, 2},
    [0x50] = {&branch_overflow_clear, &addr_unused, 2},
    [0x70] = {&branch_overflow_set, &addr_unused, 2},

    // And
    [0x29] = {&bitwise_and, &addr_immediate, 2},
    [0x25] = {&bitwise_and, &addr_zp, 3},
    [0x35] = {&bitwise_and, &addr_zp_x, 4},
    [0x2D] = {&bitwise_and, &addr_abs, 4},
    [0x3D] = {&bitwise_and, &addr_abs_x, 4},
    [0x39] = {&bitwise_and, &addr_abs_y, 4},
    [0x21] = {&bitwise_and, &addr_ind_x_pre, 6},
    [0x31] = {&bitwise_and, &addr_ind_y_post, 5},

    // Or
    [0x09] = {&bitwise_or, &addr_immediate, 2},
    [0x05] = {&bitwise_or, &addr_zp, 3},
    [0x15] = {&bitwise_or, &addr_zp_x, 4},
    [0x0D] = {&bitwise_or, &addr_abs, 4},
    [0x1D] = {&bitwise_or, &addr_abs_x, 4},
    [0x19] = {&bitwise_or, &addr_abs_y, 4},
    [0x01] = {&bitwise_or, &addr_ind_x_pre, 6},
    [0x11] = {&bitwise_or, &addr_ind_y_post, 5},

    // Exclusive Or
    [0x49] = {&bitwise_xor, &addr_immediate, 2},
    [0x45] = {&bitwise_xor, &addr_zp, 3},
    [0x55] = {&bitwise_xor, &addr_zp_x, 4},
    [0x4D] = {&bitwise_xor, &addr_abs, 4},
    [0x5D] = {&bitwise_xor, &addr_abs_x, 4},
    [0x59] = {&bitwise_xor, &addr_abs_y, 4},
    [0x41] = {&bitwise_xor, &addr_ind_x_pre, 6},
    [0x51] = {&bitwise_xor, &addr_ind_y_post, 5},

    // Bit Test
    [0x24] = {&bit_test, &addr_zp, 3},
    [0x2C] = {&bit_test, &addr_abs, 4},

    // Increment
    [0xE6] = {&increment_memory, &addr_zp, 5},
    [0xF6] = {&increment_memory, &addr_zp_x, 6},
    [0xEE] = {&increment_memory, &addr_abs, 6},
    [0xFE] = {&increment_memory, &addr_abs_x, 7},
    [0xE8] = {&increment_x, &addr_unused, 2},
    [0xC8] = {&increment_y, &addr_unused, 2},

    // Decrement
    [0xC6] = {&decrement_memory, &addr_zp, 5},
    [0xD6] = {&decrement_memory, &addr_zp_x, 6},
    [0xCE] = {&decrement_memory, &addr_abs, 6},
    [0xDE] = {&decrement_memory, &addr_abs_x, 7},
    [0xCA] = {&decrement_x, &addr_unused, 2},
    [0x88] = {&decrement_y, &addr_unused, 2},

    // SHIFT LEFT
    [0x0A] = {&shift_left_acc, &addr_unused, 2},
    [0x06] = {&shift_left, &addr_zp, 5},
    [0x16] = {&shift_left, &addr_zp_x, 6},
    [0x0E] = {&shift_left, &addr_abs, 6},
    [0x1E] = {&shift_left, &addr_abs_x, 7},

    // SHIFT RIGHT
    [0x4A] = {&shift_right_acc, &addr_unused, 2},
    [0x46] = {&shift_right, &addr_zp, 5},
    [0x56] = {&shift_right, &addr_zp_x, 6},
    [0x4E] = {&shift_right, &addr_abs, 6},
    [0x5E] = {&shift_right, &addr_abs_x, 7},

    // Rotate LEFT
    [0x2A] = {&rotate_left_acc, &addr_unused, 2},
    [0x26] = {&rotate_left, &addr_zp, 5},
    [0x36] = {&rotate_left, &addr_zp_x, 6},
    [0x2E] = {&rotate_left, &addr_abs, 6},
    [0x3E] = {&rotate_left, &addr_abs_x, 7},

    // rotate RIGHT
    [0x6A] = {&rotate_right_acc, &addr_unused, 2},
    [0x66] = {&rotate_right, &addr_zp, 5},
    [0x76] = {&rotate_right, &addr_zp_x, 6},
    [0x6E] = {&rotate_right, &addr_abs, 6},
    [0x7E] = {&rotate_right, &addr_abs_x, 7},
};