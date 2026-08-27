#include "cpu.h"
#include "opcodes.h"


void update_zn_flags(Cpu *cpu, uint8_t value){

    cpu->p &= ~(Z_BIT | N_BIT);

    cpu->p |= (value & N_BIT);

    cpu->p |= (value == 0 ? Z_BIT : 0);
}

void update_alu_flags(Cpu *cpu,uint8_t accumulator ,uint8_t value ,uint16_t result){


    cpu->p &= ~(C_BIT | Z_BIT | V_BIT | N_BIT);
    cpu->p |= (result > 0xFF ? C_BIT : 0);

    cpu->p |= ((uint8_t)result == 0 ? Z_BIT : 0) | ((uint8_t)result & N_BIT);

    cpu->p |= ((((uint8_t)result ^ accumulator) & ((uint8_t)result ^ value) & 0x80) >> 1);



}



void cpu_reset(Cpu *cpu){
    cpu->sp -= 3;
    uint8_t low_byte = bus_read(cpu->bus,RESET_VECTOR_ADDRESS_LOW);
    uint8_t high_byte = bus_read(cpu->bus,RESET_VECTOR_ADDRESS_HIGH);

    cpu->pc = ((high_byte << 8) | low_byte);
    cpu->p = cpu->p | I_DISABLE_BIT;


}


void cpu_init(Cpu *cpu, Bus* bus){

    cpu->bus = bus;
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFD;
    uint8_t low_byte = bus_read(cpu->bus,RESET_VECTOR_ADDRESS_LOW);
    uint8_t high_byte = bus_read(cpu->bus,RESET_VECTOR_ADDRESS_HIGH);

    cpu->pc = ((high_byte << 8) | low_byte);
    cpu->p = 0;
    cpu->p = cpu->p | I_DISABLE_BIT | U_BIT; 



}



void cpu_clock(Cpu *cpu){

    if(cpu->cycles_remaining != 0){
        cpu->cycles_remaining -= 1;
        return;
    }

    uint8_t opcode = bus_read(cpu->bus, cpu->pc++);

    Instruction instruction = opcodes[opcode];

    cpu->cycles_remaining = instruction.cycles -1;
    instruction.opcode_func(cpu);   

    return;
}