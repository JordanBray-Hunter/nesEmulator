#pragma once 
#include <stdint.h>
#include "bus.h"



#define C_BIT 0x01
#define Z_BIT (1 << 1)
#define I_DISABLE_BIT (1 << 2)
#define D_BIT (1 << 3)
#define B_BIT (1 << 4)
#define U_BIT (1 << 5) 
#define V_BIT (1 << 6)
#define N_BIT (1 << 7)

#define RESET_VECTOR_ADDRESS_LOW 0xFFFC
#define RESET_VECTOR_ADDRESS_HIGH 0xFFFD


typedef struct cpu
{
    uint8_t x, y , a , p, sp;
    uint16_t pc; 
    Bus* bus;

    uint8_t cycles_remaining;
} Cpu;



void update_zn_flags(Cpu *cpu, uint8_t value);



void cpu_init(Cpu *cpu, Bus* bus);







