#pragma once
#include <stdint.h>
#include "cartridge.h"
#include <raylib.h>


#define NMI_ENABLE_BIT (1 << 7 )
#define BG_ENABLE_BIT (1 << 3)

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
    bool latch;
    uint16_t PPUSCROLL; //internal 2-byte state accessed by two 1-byte accesses
    uint16_t PPUADDR; // internal 2-byte state accessed by two 1-byte accesses
    uint8_t PPUDATA;
    uint8_t OAMDMA;
    Color pixels[256 * 240];
    bool odd_frame;
    uint16_t scan_line;
    uint16_t dot;
    RenderTexture2D gameTexture;
    bool frame_ready;

    Cartridge* cartridge;
} Ppu;



void ppu_init(Ppu *ppu, Cartridge *cartridge);


void ppu_clock(Ppu *ppu);


void draw_chrs_to_texture(RenderTexture2D *texture, uint8_t *chr_rom);


uint8_t ppu_read(Ppu *ppu, uint16_t address);

void ppu_write(Ppu *ppu, uint16_t address, uint8_t value);