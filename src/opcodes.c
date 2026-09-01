#include "opcodes.h"
#include "cpu.h"
#include "addressing_modes.h"
#include <stdio.h>

void no_op(Cpu *cpu)
{
    return;
}

void load_a_immediate(Cpu *cpu)
{
    cpu->a = bus_read(cpu->bus, cpu->pc++);
    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    cpu->a = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_zp_x(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    cpu->a = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_abs(Cpu *cpu)
{
    cpu->a = bus_read(cpu->bus, addr_abs(cpu));
    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_abs_x(Cpu *cpu)
{
    cpu->a = bus_read(cpu->bus, addr_abs_x(cpu, true));
    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_abs_y(Cpu *cpu)
{

    cpu->a = bus_read(cpu->bus, addr_abs_y(cpu, true));
    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_ind_x_pre(Cpu *cpu)
{

    cpu->a = bus_read(cpu->bus, addr_ind_x_pre(cpu));

    update_zn_flags(cpu, cpu->a);
    return;
}

void load_a_ind_y_post(Cpu *cpu)
{
    cpu->a = bus_read(cpu->bus, addr_ind_y_post(cpu, true));
    update_zn_flags(cpu, cpu->a);

    return;
}

// Load X

void load_x_immediate(Cpu *cpu)
{
    cpu->x = bus_read(cpu->bus, cpu->pc++);
    update_zn_flags(cpu, cpu->x);
    return;
}

void load_x_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    cpu->x = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->x);
    return;
}

void load_x_zp_y(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->y;
    cpu->x = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->x);
    return;
}

void load_x_abs(Cpu *cpu)
{
    cpu->x = bus_read(cpu->bus, addr_abs(cpu));
    update_zn_flags(cpu, cpu->x);
    return;
}

void load_x_abs_y(Cpu *cpu)
{

    cpu->x = bus_read(cpu->bus, addr_abs_y(cpu, true));
    update_zn_flags(cpu, cpu->x);
    return;
}

// Load y

void load_y_immediate(Cpu *cpu)
{
    cpu->y = bus_read(cpu->bus, cpu->pc++);
    update_zn_flags(cpu, cpu->y);
    return;
}

void load_y_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    cpu->y = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->y);
    return;
}

void load_y_zp_x(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    cpu->y = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->y);
    return;
}

void load_y_abs(Cpu *cpu)
{
    cpu->y = bus_read(cpu->bus, addr_abs(cpu));
    update_zn_flags(cpu, cpu->y);
    return;
}

void load_y_abs_x(Cpu *cpu)
{

    cpu->y = bus_read(cpu->bus, addr_abs_x(cpu, true));
    update_zn_flags(cpu, cpu->y);
    return;
}

// Store A

void store_a_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);

    bus_write(cpu->bus, address, cpu->a);
}

void store_a_zp_x(Cpu *cpu)
{

    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    bus_write(cpu->bus, address, cpu->a);
}

void store_a_abs(Cpu *cpu)
{
    uint16_t address = addr_abs(cpu);

    bus_write(cpu->bus, address, cpu->a);
}

void store_a_abs_x(Cpu *cpu)
{
    uint16_t address = addr_abs_x(cpu, false);

    bus_write(cpu->bus, address, cpu->a);
}

void store_a_abs_y(Cpu *cpu)
{
    uint16_t address = addr_abs_y(cpu, false);
    bus_write(cpu->bus, address, cpu->a);
}

void store_a_ind_x_pre(Cpu *cpu)
{
    uint16_t address = addr_ind_x_pre(cpu);
    bus_write(cpu->bus, address, cpu->a);
}

void store_a_ind_y_post(Cpu *cpu)
{
    uint16_t address = addr_ind_y_post(cpu, false);
    bus_write(cpu->bus, address, cpu->a);
}

// Store X

void store_x_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);

    bus_write(cpu->bus, address, cpu->x);
}

void store_x_zp_y(Cpu *cpu)
{

    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->y;
    bus_write(cpu->bus, address, cpu->x);
}

void store_x_abs(Cpu *cpu)
{
    uint16_t address = addr_abs(cpu);

    bus_write(cpu->bus, address, cpu->x);
}

// Store Y

void store_y_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);

    bus_write(cpu->bus, address, cpu->y);
}

void store_y_zp_x(Cpu *cpu)
{

    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    bus_write(cpu->bus, address, cpu->y);
}

void store_y_abs(Cpu *cpu)
{
    uint16_t address = addr_abs(cpu);

    bus_write(cpu->bus, address, cpu->y);
}

// Add with carry

void add_with_carry_immediate(Cpu *cpu)
{

    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t value = bus_read(cpu->bus, cpu->pc++);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_zp(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t address = bus_read(cpu->bus, cpu->pc++);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_zp_x(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_abs(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs(cpu);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_abs_x(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs_x(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_abs_y(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs_y(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_ind_x_pre(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_ind_x_pre(cpu);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_ind_y_post(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_ind_y_post(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

// Sub with carry

void sub_with_carry_immediate(Cpu *cpu)
{

    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t value = bus_read(cpu->bus, cpu->pc++);

    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_zp(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t address = bus_read(cpu->bus, cpu->pc++);

    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_zp_x(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_abs(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs(cpu);

    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_abs_x(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs_x(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_abs_y(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs_y(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_ind_x_pre(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_ind_x_pre(cpu);

    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void sub_with_carry_ind_y_post(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_ind_y_post(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);
    value = (uint8_t)(~value);

    uint16_t result = cpu->a + value + carry_in;

    update_alu_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

// Transfer

void transfer_a_to_x(Cpu *cpu)
{
    cpu->x = cpu->a;
    update_zn_flags(cpu, cpu->x);
}

void transfer_x_to_a(Cpu *cpu)
{
    cpu->a = cpu->x;
    update_zn_flags(cpu, cpu->a);
}

void transfer_a_to_y(Cpu *cpu)
{
    cpu->y = cpu->a;
    update_zn_flags(cpu, cpu->y);
}

void transfer_y_to_a(Cpu *cpu)
{
    cpu->a = cpu->y;
    update_zn_flags(cpu, cpu->a);
}

void transfer_x_to_sp(Cpu *cpu)
{
    cpu->sp = cpu->x;
}

void transfer_sp_to_x(Cpu *cpu)
{
    cpu->x = cpu->sp;
    update_zn_flags(cpu, cpu->x);
}

// Flags

void clear_carry(Cpu *cpu)
{

    cpu->p &= ~(C_BIT);
}

void clear_interrupt_disable(Cpu *cpu)
{

    cpu->p &= ~(I_DISABLE_BIT);
}

void clear_overflow(Cpu *cpu)
{
    cpu->p &= ~(V_BIT);
}

void clear_decimal(Cpu *cpu)
{
    cpu->p &= ~(D_BIT);
}

void set_decimal(Cpu *cpu)
{

    cpu->p |= D_BIT;
}

void set_interrupt_disable(Cpu *cpu)
{

    cpu->p |= I_DISABLE_BIT;
}

void set_carry(Cpu *cpu)
{
    cpu->p |= C_BIT;
}

// Stack

void push_a_to_stack(Cpu *cpu)
{

    bus_write(cpu->bus, (0x0100 + cpu->sp), cpu->a);
    cpu->sp--;
}

void pull_a_from_stack(Cpu *cpu)
{
    cpu->sp++;
    cpu->a = bus_read(cpu->bus, 0x0100 + cpu->sp);
    update_zn_flags(cpu, cpu->a);
}

void push_p_to_stack(Cpu *cpu)
{
    bus_write(cpu->bus, (0x0100 + cpu->sp), (cpu->p) | (1 << 5) | (1 << 4));
    cpu->sp--;
}

void pull_p_from_stack(Cpu *cpu)
{

    cpu->sp++;
    cpu->sp++;
    cpu->p = bus_read(cpu->bus, 0x0100 + cpu->sp) | (1 << 5);
}

// Jump

void jump_abs(Cpu *cpu)
{
    cpu->pc = addr_abs(cpu);
}

void jump_ind(Cpu *cpu)
{
    uint8_t low = bus_read(cpu->bus, cpu->pc++);
    uint8_t high = bus_read(cpu->bus, cpu->pc++);
    uint16_t address1 = (high << 8) | low;
    low++;
    uint16_t address2 = (high << 8) | low;

    uint8_t target_low = bus_read(cpu->bus, address1);
    uint8_t target_high = bus_read(cpu->bus, address2);

    cpu->pc = ((uint16_t)target_high << 8) | target_low;
}

void jump_to_sub(Cpu *cpu)
{

    bus_write(cpu->bus, (0x0100 + cpu->sp), ((cpu->pc + 1) & 0xFF00) >> 8);
    cpu->sp--;
    bus_write(cpu->bus, (0x0100 + cpu->sp), ((cpu->pc + 1) & 0x00FF));
    cpu->sp--;
    cpu->pc = addr_abs(cpu);
}

void return_from_sub(Cpu *cpu)
{

    cpu->sp++;
    uint8_t low = bus_read(cpu->bus, 0x0100 + cpu->sp);
    cpu->sp++;
    uint8_t high = bus_read(cpu->bus, 0x0100 + cpu->sp);
    cpu->pc = ((high << 8) | low) + 1;
}

// Compare A

void compare_a_immediate(Cpu *cpu)
{
    uint8_t value = bus_read(cpu->bus, cpu->pc++);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_zp_x(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_abs(Cpu *cpu)
{
    uint16_t address = addr_abs(cpu);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_abs_x(Cpu *cpu)
{
    uint16_t address = addr_abs_x(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_abs_y(Cpu *cpu)
{
    uint16_t address = addr_abs_y(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_ind_x_pre(Cpu *cpu)
{
    uint16_t address = addr_ind_x_pre(cpu);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

void compare_a_ind_y_post(Cpu *cpu)
{
    uint16_t address = addr_ind_y_post(cpu, true);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->a, value);
}

// Compare X

void compare_x_immediate(Cpu *cpu)
{
    uint8_t value = bus_read(cpu->bus, cpu->pc++);
    compare_flags(cpu, cpu->x, value);
}

void compare_x_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->x, value);
}

void compare_x_abs(Cpu *cpu)
{
    uint16_t address = addr_abs(cpu);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->x, value);
}

// Compare Y

void compare_y_immediate(Cpu *cpu)
{
    uint8_t value = bus_read(cpu->bus, cpu->pc++);
    compare_flags(cpu, cpu->y, value);
}

void compare_y_zp(Cpu *cpu)
{
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->y, value);
}

void compare_y_abs(Cpu *cpu)
{
    uint16_t address = addr_abs(cpu);
    uint8_t value = bus_read(cpu->bus, address);
    compare_flags(cpu, cpu->y, value);
}

// Branch

void branch_carry_clear(Cpu *cpu)
{
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

void branch_carry_set(Cpu *cpu)
{
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

void branch_equal(Cpu *cpu)
{
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
void branch_not_equal(Cpu *cpu)
{
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

void branch_plus(Cpu *cpu)
{
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
void branch_minus(Cpu *cpu)
{
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

void branch_overflow_clear(Cpu *cpu)
{
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
void branch_overflow_set(Cpu *cpu)
{
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

Instruction opcodes[256] = {

    // NOP
    [0xEA] = {&no_op, 2},
    // LDA
    [0xA9] = {&load_a_immediate, 2},
    [0xA5] = {&load_a_zp, 3},
    [0xB5] = {&load_a_zp_x, 4},
    [0xAD] = {&load_a_abs, 4},
    [0xBD] = {&load_a_abs_x, 4},
    [0xB9] = {&load_a_abs_y, 4},
    [0xA1] = {&load_a_ind_x_pre, 6},
    [0xB1] = {&load_a_ind_y_post, 5},

    // STA
    [0x85] = {&store_a_zp, 3},
    [0x95] = {&store_a_zp_x, 4},
    [0x8D] = {&store_a_abs, 4},
    [0x9D] = {&store_a_abs_x, 5},
    [0x99] = {&store_a_abs_y, 5},
    [0x81] = {&store_a_ind_x_pre, 6},
    [0x91] = {&store_a_ind_y_post, 6},

    // LDX

    [0xA2] = {&load_x_immediate, 2},
    [0xA6] = {&load_x_zp, 3},
    [0xB6] = {&load_x_zp_y, 4},
    [0xAE] = {&load_x_abs, 4},
    [0xBE] = {&load_x_abs_y, 4},

    // STX
    [0x86] = {&store_x_zp, 3},
    [0x96] = {&store_x_zp_y, 4},
    [0x8E] = {&store_x_abs, 4},

    // LDY
    [0xA0] = {&load_y_immediate, 2},
    [0xA4] = {&load_y_zp, 3},
    [0xB4] = {&load_y_zp_x, 4},
    [0xAC] = {&load_y_abs, 4},
    [0xBC] = {&load_y_abs_x, 4},

    // STY
    [0x84] = {&store_y_zp, 3},
    [0x94] = {&store_y_zp_x, 4},
    [0x8C] = {&store_y_abs, 4},

    // ADC

    [0x69] = {&add_with_carry_immediate, 2},
    [0x65] = {&add_with_carry_zp, 3},
    [0x75] = {&add_with_carry_zp_x, 4},
    [0x6D] = {&add_with_carry_abs, 4},
    [0x7D] = {&add_with_carry_abs_x, 4},
    [0x79] = {&add_with_carry_abs_y, 4},
    [0x61] = {&add_with_carry_ind_x_pre, 6},
    [0x71] = {&add_with_carry_ind_y_post, 5},

    // SBC
    [0xE9] = {&sub_with_carry_immediate, 2},
    [0xE5] = {&sub_with_carry_zp, 3},
    [0xF5] = {&sub_with_carry_zp_x, 4},
    [0xED] = {&sub_with_carry_abs, 4},
    [0xFD] = {&sub_with_carry_abs_x, 4},
    [0xF9] = {&sub_with_carry_abs_y, 4},
    [0xE1] = {&sub_with_carry_ind_x_pre, 6},
    [0xF1] = {&sub_with_carry_ind_y_post, 5},

    // Transfer
    [0xAA] = {&transfer_a_to_x, 2},
    [0xA8] = {&transfer_a_to_y, 2},
    [0xBA] = {&transfer_sp_to_x, 2},
    [0x8A] = {&transfer_x_to_a, 2},
    [0x9A] = {&transfer_x_to_sp, 2},
    [0x98] = {&transfer_y_to_a, 2},

    // Flags

    [0x18] = {&clear_carry, 2},
    [0x38] = {&set_carry, 2},
    [0x58] = {&clear_interrupt_disable, 2},
    [0x78] = {&set_interrupt_disable, 2},
    [0xD8] = {&clear_decimal, 2},
    [0xF8] = {&set_decimal, 2},
    [0xB8] = {&clear_overflow, 2},

    // Stack
    [0x48] = {&push_a_to_stack, 3},
    [0x68] = {&pull_a_from_stack, 4},
    [0x08] = {&push_p_to_stack, 3},
    [0x28] = {&pull_p_from_stack, 4},

    // Jump
    [0x4C] = {&jump_abs, 3},
    [0x6C] = {&jump_ind, 5},
    [0x20] = {&jump_to_sub, 6},
    [0x60] = {&return_from_sub, 6},

    // Compare A

    [0xC9] = {&compare_a_immediate, 2},
    [0xC5] = {&compare_a_zp, 3},
    [0xD5] = {&compare_a_zp_x, 4},
    [0xCD] = {&compare_a_abs, 4},
    [0xDD] = {&compare_a_abs_x, 4},
    [0xD9] = {&compare_a_abs_y, 4},
    [0xC1] = {&compare_a_ind_x_pre, 6},
    [0xD1] = {&compare_a_ind_y_post, 5},

    // Compare X

    [0xE0] = {&compare_x_immediate, 2},
    [0xE4] = {&compare_x_zp, 3},
    [0xEC] = {&compare_x_abs, 4},

    // Compare Y

    [0xC0] = {&compare_y_immediate, 2},
    [0xC4] = {&compare_y_zp, 3},
    [0xCC] = {&compare_y_abs, 4},

    // Branch

    [0x90] = {&branch_carry_clear, 2},
    [0xB0] = {&branch_carry_set, 2},
    [0xF0] = {&branch_equal, 2},
    [0xD0] = {&branch_not_equal, 2},
    [0x10] = {&branch_plus, 2},
    [0x30] = {&branch_minus, 2},
    [0x50] = {&branch_overflow_clear, 2},
    [0x70] = {&branch_overflow_set, 2},
};