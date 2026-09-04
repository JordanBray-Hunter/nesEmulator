#include "addressing_modes.h"


uint16_t addr_ind_x_pre(Cpu* cpu){
        uint8_t value = bus_read(cpu->bus, cpu->pc++);
    value += cpu->x;
    uint8_t low = bus_read(cpu->bus, value);
    value += 1;
    uint8_t high = bus_read(cpu->bus, value);
    return (high << 8) | low;
}

uint16_t addr_abs(Cpu *cpu){
        uint16_t low = bus_read(cpu->bus, cpu->pc++);
    uint16_t high = bus_read(cpu->bus, cpu->pc++);
    uint16_t address = (high << 8) | low;
    return address;
}


uint16_t addr_ind_y_post(Cpu *cpu,bool can_add_extra_cycle)
{
    uint8_t value = bus_read(cpu->bus, cpu->pc++);
    uint8_t low = bus_read(cpu->bus, value);
    value += 1; // has to be here so it wraps correctly, if done in read it casts to uint16 and does not wrap
    uint8_t high = bus_read(cpu->bus, value);
    uint16_t address = (high << 8) | low;
    address += cpu->y;
    if (can_add_extra_cycle && ((high << 8) != (address & 0xFF00)))
    {
        cpu->cycles_remaining++;
    };
    return address;
}

uint16_t addr_abs_x(Cpu *cpu,bool can_add_extra_cycle){
    uint16_t low = bus_read(cpu->bus, cpu->pc++);
    uint16_t high = bus_read(cpu->bus, cpu->pc++);
    uint16_t address = (high << 8) | low;
    address += cpu->x;

 if (can_add_extra_cycle && ((high << 8) != (address & 0xFF00)))
    {
        cpu->cycles_remaining++;
    };
    return address;
}




uint16_t addr_abs_y(Cpu *cpu,bool can_add_extra_cycle){

     uint16_t low = bus_read(cpu->bus, cpu->pc++);
    uint16_t high = bus_read(cpu->bus, cpu->pc++);
    uint16_t address = (high << 8) | low;
    address += cpu->y;

 if (can_add_extra_cycle && ((high << 8) != (address & 0xFF00)))
    {
        cpu->cycles_remaining++;
    };
    return address;
}