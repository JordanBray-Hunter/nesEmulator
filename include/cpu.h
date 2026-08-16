#pragma once 
#include <stdint.h>
#include "bus.h"

typedef struct cpu
{
    uint8_t x, y , a , p, sp;
    uint16_t pc; 
    Bus* bus;
} Cpu;






void cpu_init(Cpu *cpu, Bus* bus);







