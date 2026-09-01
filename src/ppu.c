
#include "ppu.h"
#include <stdio.h>

void ppu_init(Ppu *ppu, Cartridge *cartridge)
{

    ppu->cartridge = cartridge;
    ppu->gameTexture = LoadRenderTexture(256, 240);

    ppu->dot = 0;
    ppu->scan_line = 0;
}

Color palette[4] = {BLACK, RED, GREEN, BLUE};

void ppu_clock(Ppu *ppu)
{

    if (ppu->scan_line < 240 && ppu->dot < 256)
    {
        int value = GetRandomValue(0, 3);
        ppu->pixels[ppu->scan_line * 256 + ppu->dot] = palette[value];
    }

    ppu->dot++;
    if (ppu->dot >= 341)
    {
        ppu->dot = 0;
        ppu->scan_line++;

        if (ppu->scan_line == 241)
        {
            ppu->frame_ready = true;
        }

        if (ppu->scan_line >= 262) 
        {
            ppu->scan_line = 0;
        }
    }
}

uint8_t ppu_read(Ppu *ppu, uint16_t address){

}

void ppu_write(Ppu *ppu, uint16_t address, uint8_t value){

}



// Debug function to draw entire char rom to texture
void draw_chrs_to_texture(RenderTexture2D *texture, uint8_t *chr_rom)
{
    BeginTextureMode(*texture);
    ClearBackground(BLACK);

    for (int tile_num = 0; tile_num < 256; tile_num++)
    {

        for (int row = 0; row < 8; row++)
        {
            int address1 = (0 << 12) | ((uint8_t)tile_num << 4) | (0 << 3) | row;
            int address2 = (0 << 12) | ((uint8_t)tile_num << 4) | (1 << 3) | row;
            uint8_t plane1 = chr_rom[address1];
            uint8_t plane2 = chr_rom[address2];
            for (int pixel = 0; pixel < 8; pixel++)
            {
                uint8_t bit0 = (plane1 >> (7 - pixel)) & 1;
                uint8_t bit1 = (plane2 >> (7 - pixel)) & 1;
                uint8_t color = (bit1 << 1) | bit0;
                int tile_x = (tile_num % 16) * 8 + pixel;
                int tile_y = (tile_num / 16) * 8 + row;

                DrawPixel(tile_x, tile_y, palette[color]);
            }
        }
    }

    for (int tile_num = 0; tile_num < 256; tile_num++)
    {

        for (int row = 0; row < 8; row++)
        {
            int address1 = (1 << 12) | ((uint8_t)tile_num << 4) | (0 << 3) | row;
            int address2 = (1 << 12) | ((uint8_t)tile_num << 4) | (1 << 3) | row;
            uint8_t plane1 = chr_rom[address1];
            uint8_t plane2 = chr_rom[address2];
            for (int pixel = 0; pixel < 8; pixel++)
            {
                uint8_t bit0 = (plane1 >> (7 - pixel)) & 1;
                uint8_t bit1 = (plane2 >> (7 - pixel)) & 1;
                uint8_t color = (bit1 << 1) | bit0;
                int tile_x = (tile_num % 16) * 8 + pixel + (8 * 16);
                int tile_y = (tile_num / 16) * 8 + row;

                DrawPixel(tile_x, tile_y, palette[color]);
            }
        }
    }

    EndTextureMode();
}
