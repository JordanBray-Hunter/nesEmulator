#pragma once
#include <stdint.h>
#include "cartridge.h"
#include <raylib.h>
#include "cpu.h"


#define NMI_ENABLE_BIT (1 << 7 )
#define V_BLANK_BIT (1 << 7)
#define BG_ENABLE_BIT (1 << 3)
#define SPRITE_ENABLE_BIT (1 << 4)
#define VRAM_INCREMENT (1 << 3)
#define FINE_Y_BITS 7000


typedef struct cpu Cpu;


typedef struct ppu
{
    uint8_t v_ram[2048];
    uint8_t palette_ram[32];
    uint8_t oam[256];
    uint8_t PPUCTRL;
    uint8_t PPUMASK;
    uint8_t PPUSTATUS;
    uint8_t OAMADDR;
    Cpu *cpu;

    uint8_t OAMDATA;
    bool write_toggle;
    uint16_t PPUSCROLL; //internal 2-byte state accessed by two 1-byte accesses
    uint16_t PPUADDR; // internal 2-byte state accessed by two 1-byte accesses
    uint8_t PPUDATA;
    uint8_t ppu_data_buffer;

    // internal reg
    uint16_t vram_address, temp_vram_address;
    uint8_t fine_x;


    uint8_t OAMDMA;
    Color pixels[256 * 240];
    bool odd_frame;
    uint16_t scan_line;
    uint16_t dot;
    RenderTexture2D gameTexture;
    bool frame_ready;

    uint8_t next_nametable_id;
    uint8_t next_nametable_attr;
    uint8_t next_lsb_plane, next_msb_plane;


    uint16_t shift_register_attrbute_lsb, shift_register_attrbute_msb; 
    uint16_t shift_register_lsb_plane, shift_register_msb_plane;



    Cartridge* cartridge;
} Ppu;

uint8_t ppu_vram_read(Ppu *ppu, uint16_t address);


void ppu_init(Ppu *ppu, Cartridge *cartridge, Cpu *cpu);


void ppu_clock(Ppu *ppu);


void draw_chrs_to_texture(RenderTexture2D *texture, uint8_t *chr_rom);

//Google ai made this. 
void print_nametables_to_console(Ppu *ppu);


uint8_t ppu_read(Ppu *ppu, uint16_t address);

void ppu_write(Ppu *ppu, uint16_t address, uint8_t value);