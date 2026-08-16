#pragma once
#include <stdint.h>
#include "cartridge.h"

typedef struct ppu
{
    uint8_t v_ram[2048];
    uint8_t palette_ram[32];
    uint8_t oam[256];
    uint8_t PPUCTRL;
    uint8_t PPUMASK;
    uint8_t PPUSTATUS;
    uint8_t OAMADDR;
    uint8_t OAMDATA;
    uint8_t PPUSCROLL; //internal 2-byte state accessed by two 1-byte accesses
    uint8_t PPUADDR; // internal 2-byte state accessed by two 1-byte accesses
    uint8_t PPUDATA;
    uint8_t OAMDMA;

    Cartridge* cartridge;
} Ppu;




