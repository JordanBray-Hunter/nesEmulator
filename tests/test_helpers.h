// test_helpers.h
#pragma once
#include "cpu.h"
#include "bus.h"
#include "ppu.h"
#include "cartridge.h"

typedef struct test_system {
    Cpu cpu;
    Bus bus;
    Ppu ppu;
    Cartridge cartridge;
} TestSystem;
void make_test_system(TestSystem *sys);


void free_test_system(TestSystem *sys);

// Loads `len` bytes into PRG ROM starting at `address` (must be >= 0x8000)
void load_program(TestSystem *sys, uint16_t address, const uint8_t *program, size_t len);