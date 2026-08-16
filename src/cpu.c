#include "cpu.h"

#define C_BIT 0x01
#define I_DISABLE_BIT (1 << 2)
#define Z_BIT (1 << 1)
#define V_BIT (1 << 6)
#define N_BIT (1 << 7)
#define D_BIT (1 << 3)
#define B_BIT (1 << 4)
#define U_BIT (1 << 5) 


void cpu_init(Cpu *cpu, Bus* bus){

    cpu->bus = bus;
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFD;
    cpu->pc = 0xFFFC;

    cpu->p = 0;
    cpu->p = cpu->p | I_DISABLE_BIT | U_BIT;




}