#pragma once 

#include "cpu.h"
#include <stdint.h>
#include <stdbool.h>


uint16_t addr_ind_x_pre(Cpu* cpu);

uint16_t addr_abs(Cpu *cpu);

uint16_t addr_ind_y_post(Cpu *cpu,bool is_reading);

uint16_t addr_abs_x(Cpu *cpu,bool is_reading);


uint16_t addr_abs_y(Cpu *cpu,bool is_reading);








