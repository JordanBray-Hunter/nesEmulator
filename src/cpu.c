#include "cpu.h"



void update_zn_flags(Cpu *cpu, uint8_t value){

    cpu->p &= ~(Z_BIT | N_BIT);

    cpu->p |= (value & N_BIT);

    cpu->p |= (value == 0 ? Z_BIT : 0);
}

void cpu_init(Cpu *cpu, Bus* bus){

    cpu->bus = bus;
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFD;
    uint8_t low_byte = bus_read(bus,RESET_VECTOR_ADDRESS_LOW);
    uint8_t high_byte = bus_read(bus,RESET_VECTOR_ADDRESS_HIGH);

    cpu->pc = ((high_byte << 8) | low_byte);
    cpu->p = 0;
    cpu->p = cpu->p | I_DISABLE_BIT | U_BIT;




}