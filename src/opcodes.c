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

//Load X


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

//Load y 

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

void add_with_carry_immediate(Cpu *cpu)
{

    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t value = bus_read(cpu->bus, cpu->pc++);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_zp(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t address = bus_read(cpu->bus, cpu->pc++);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_zp_x(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_abs(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs(cpu);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_abs_x(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs_x(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_abs_y(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_abs_y(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_ind_x_pre(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_ind_x_pre(cpu);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
}

void add_with_carry_ind_y_post(Cpu *cpu)
{
    uint8_t carry_in = cpu->p & C_BIT ? 1 : 0;
    uint16_t address = addr_ind_y_post(cpu, true);

    uint8_t value = bus_read(cpu->bus, address);

    uint16_t result = cpu->a + value + carry_in;

    update_adc_flags(cpu, cpu->a, value, result);

    cpu->a = (uint8_t)result;
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

    // ADC

    [0x69] = {&add_with_carry_immediate, 2},
    [0x65] = {&add_with_carry_zp, 3},
    [0x75] = {&add_with_carry_zp_x, 4},
    [0x6D] = {&add_with_carry_abs, 4},
    [0x7D] = {&add_with_carry_abs_x, 4},
    [0x79] = {&add_with_carry_abs_y, 4},
    [0x61] = {&add_with_carry_ind_x_pre, 6},
    [0x71] = {&add_with_carry_ind_y_post, 5},

};