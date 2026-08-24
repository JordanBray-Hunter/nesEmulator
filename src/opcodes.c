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
    cpu->a = bus_read(cpu->bus, addr_abs_x(cpu,true));
    update_zn_flags(cpu, cpu->a);
    return;
}




void load_a_abs_y(Cpu *cpu)
{
   

    cpu->a = bus_read(cpu->bus, addr_abs_y(cpu,true));
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
    cpu->a = bus_read(cpu->bus, addr_ind_y_post(cpu,true));
    update_zn_flags(cpu, cpu->a);

    return;
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

    //

};