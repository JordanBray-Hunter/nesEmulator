#pragma once

#include "cpu.h"
#include <stdint.h>
#include <stdbool.h>

uint16_t addr_unused(Cpu *_, bool __);

uint16_t addr_indirect(Cpu *cpu, bool _);
uint16_t addr_immediate(Cpu *cpu, bool _);

uint16_t addr_zp(Cpu *cpu, bool _);

uint16_t addr_zp_x(Cpu *cpu, bool _);
uint16_t addr_zp_y(Cpu *cpu, bool _);

uint16_t addr_ind_x_pre(Cpu *cpu, bool _);

uint16_t addr_abs(Cpu *cpu, bool _);

uint16_t addr_ind_y_post(Cpu *cpu, bool is_reading);

uint16_t addr_abs_x(Cpu *cpu, bool is_reading);

uint16_t addr_abs_y(Cpu *cpu, bool is_reading);
