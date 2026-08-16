#include "opcodes.h"
#include "cpu.h"

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

void load_a_zp_x(Cpu *cpu){
    uint8_t address = bus_read(cpu->bus, cpu->pc++);
    address += cpu->x;
    cpu->a = bus_read(cpu->bus, address);
    update_zn_flags(cpu, cpu->a );
    return;
}


void load_a_abs(Cpu *cpu){
    uint16_t low = bus_read(cpu->bus,cpu->pc++);
    uint16_t high = bus_read(cpu->bus,cpu->pc++);
    uint16_t address = (high << 8) | low;

    cpu->a = bus_read(cpu->bus,address);
        update_zn_flags(cpu, cpu->a );
    return;

}

void load_a_abs_x(Cpu *cpu){
        uint16_t low = bus_read(cpu->bus,cpu->pc++);
    uint16_t high = bus_read(cpu->bus,cpu->pc++);
    uint16_t address = (high << 8) | low;
    address += cpu->x;

    if((high << 8) != (address & 0xFF00)){
        cpu->cycles_remaining++;
    }

    cpu->a = bus_read(cpu->bus,address);
        update_zn_flags(cpu, cpu->a );
    return;
}

    void load_a_abs_y(Cpu *cpu){
            uint16_t low = bus_read(cpu->bus,cpu->pc++);
        uint16_t high = bus_read(cpu->bus,cpu->pc++);
        uint16_t address = (high << 8) | low;
        address += cpu->y;

        if((high << 8) != (address & 0xFF00)){
            cpu->cycles_remaining++;
        }

        cpu->a = bus_read(cpu->bus,address);
            update_zn_flags(cpu, cpu->a );
        return;
    }



Instruction opcodes[256] = {

    //NOP
    [0xEA] = {&no_op, 2},
    //LDA
    [0xA9] = {&load_a_immediate,2},
    [0x45] = {&load_a_zp, 3},
    [0xB5] = {&load_a_zp_x, 4},
    [0xAD] = {&load_a_abs, 4},
    [0xBD] = {&load_a_abs_x, 4},
    [0xB9] = {&load_a_abs_y, 4},
    [0xA1] = {&load_a_ind_x_pre, 6},
    [0xb1] = {&load_a_ind_y_post, 5},

    //





};