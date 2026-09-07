#pragma once
#include <stdint.h>
#include "cartridge.h"
#include "ppu.h"
#include "controller.h"

#define RAM_SIZE 2048

typedef struct ppu Ppu;

typedef struct bus {
    uint8_t ram[RAM_SIZE];
    Cartridge* cartridge;
    Controller *controller;
    Ppu* ppu;
    
    uint8_t wram[0x2000]; // 8KB of Work/Save RAM


} Bus;



void bus_init(Bus* bus, Cartridge* cartridge,Ppu *ppu,Controller *controller);

uint8_t bus_read(Bus *bus, uint16_t address);

void bus_write(Bus *bus, uint16_t address, uint8_t value);




