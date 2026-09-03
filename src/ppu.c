
#include "ppu.h"
#include <stdio.h>

#define HORIZONTAL_BITMAP 0x041F
#define VERTICAL_BITMAP 0x7BE0

// Based of sudo code from https://www.nesdev.org/wiki/PPU_scrolling#PPU_internal_registers
void course_x_increment(Ppu *ppu)
{
    if ((ppu->vram_address & 0x001F) == 31)
    {
        ppu->vram_address &= ~(0x001F);
        ppu->vram_address ^= 0x0400;
    }
    else
    {
        ppu->vram_address++;
    }
}

void copy_horizontal_bits(Ppu *ppu)
{
    if ((ppu->PPUMASK & BG_ENABLE_BIT || ppu->PPUMASK & SPRITE_ENABLE_BIT))
    {

        ppu->vram_address &= ~HORIZONTAL_BITMAP;

        ppu->vram_address |= (ppu->temp_vram_address & HORIZONTAL_BITMAP);
    }
}

void copy_vertical_bits(Ppu *ppu)
{
    if ((ppu->PPUMASK & BG_ENABLE_BIT || ppu->PPUMASK & SPRITE_ENABLE_BIT))
    {

        ppu->vram_address &= ~VERTICAL_BITMAP;

        ppu->vram_address |= (ppu->temp_vram_address & VERTICAL_BITMAP);
    }
}

// Based of sudo code from https://www.nesdev.org/wiki/PPU_scrolling#PPU_internal_registers
void y_increment(Ppu *ppu)
{

    if ((ppu->vram_address & 0x7000) != 0x7000)
    {
        ppu->vram_address += 0x1000;
    }
    else
    {
        ppu->vram_address &= ~0x7000;
        int y = (ppu->vram_address & 0x03E0) >> 5;
        if (y == 29)
        {
            y = 0;
            ppu->vram_address ^= 0x0800;
        }
        else if (y == 31)
        {
            y = 0;
        }
        else
        {
            y += 1;
        }
        ppu->vram_address = (ppu->vram_address & ~0x03E0) | (y << 5);
    }
}

uint8_t ppu_vram_read(Ppu *ppu, uint16_t address)
{

    if (address >= 0x0000 && address <= 0x0FFF)
    {
        return ppu->cartridge->chr_rom[address];
    }
    else if (address >= 0x1000 && address <= 0x1FFF)
    {
        return ppu->cartridge->chr_rom[address];
    }
    else if (address >= 0x2000 && address <= 0x3EFF)
    {
        uint16_t base_address = address & 0x0FFF;

        // Need mirroring etc.
        if (ppu->cartridge->is_vertical)
        {
            // ignore bit 11 to mirror vertically
            return ppu->v_ram[base_address & (~(1 << 11))];
        }
        else
        {
            // ignore bit 10 to map horiz
            return ppu->v_ram[base_address & (~(1 << 10))];
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF)
    {
        uint16_t base_address = address & 0x001F;

        if (base_address == 0x0010 || base_address == 0x0014 || base_address == 0x0018 || base_address == 0x001C)
        {
            base_address -= 0x0010;
        }
        return ppu->palette_ram[base_address] & ((ppu->PPUMASK & (0x01)) ? 0x30 : 0xFF);
    }
    return 0;
}

void ppu_vram_write(Ppu *ppu, uint16_t address, uint8_t value)
{
    if (address >= 0x0000 && address <= 0x0FFF)
    {
        ppu->cartridge->chr_rom[address] = value;
    }
    else if (address >= 0x1000 && address <= 0x1FFF)
    {
        // Need to check if ram or rom
        ppu->cartridge->chr_rom[address] = value;
    }
    else if (address >= 0x2000 && address <= 0x3EFF)
    {
        uint16_t base_address = address & 0x0FFF;

        // Need mirroring etc.
        if (ppu->cartridge->is_vertical)
        {
            // ignore bit 11 to mirror vertically
            ppu->v_ram[base_address & (~(1 << 11))] = value;
        }
        else
        {
            // replace bit 10 with bit 11 and clear bit 11
            ppu->v_ram[base_address & (~((1 << 10) | (1 << 11))) | ((base_address & (1 << 11)) >> 1)] = value;
        }
    }
    else if (address >= 0x3F00 && address <= 0x3FFF)
    {
        uint16_t base_address = address & 0x001F;

        if (base_address == 0x0010 || base_address == 0x0014 || base_address == 0x0018 || base_address == 0x001C)
        {
            base_address -= 0x0010;
        }
        ppu->palette_ram[base_address] = value;

        return;
    }
}

void ppu_init(Ppu *ppu, Cartridge *cartridge)
{

    ppu->cartridge = cartridge;
    ppu->gameTexture = LoadRenderTexture(256, 240);

    ppu->PPUCTRL = 0;
    ppu->PPUMASK = 0;
    ppu->dot = 0;
    ppu->scan_line = 0;
}

Color palette[4] = {BLACK, RED, GREEN, BLUE};

void ppu_clock(Ppu *ppu)
{

    if (ppu->scan_line < 240 && ppu->dot < 256)
    {
        if ((ppu->PPUMASK & BG_ENABLE_BIT || ppu->PPUMASK & SPRITE_ENABLE_BIT))
        {

            int value = GetRandomValue(0, 3);
            ppu->pixels[ppu->scan_line * 256 + ppu->dot] = palette[value];
        }
        else
        {
            ppu->pixels[ppu->scan_line * 256 + ppu->dot] = WHITE;
        }
    }
    if (ppu->scan_line == 261 && ppu->dot == 1)
    {
        ppu->PPUSTATUS &= ~(V_BLANK_BIT);
    }

    if ((1 <= ppu->dot && ppu->dot < 257) || (321 <= ppu->dot && ppu->dot < 337))
    {
        // SIMILAR TO JTHIGN
        switch ((ppu->dot - 1) % 8)
        {
        // maybe needs to be diffrent.
        case 0:
            uint8_t nt = ppu_read(ppu, 0x2000 + (ppu->vram_address & 0x0FFF));

            break;
        case 2:
                 attribute address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07)
            break;
        case 4:

            break;
        case 6:
            break;
        case 8:
            break;
        
        }
    }

    if (ppu->dot == 256)
    {

        y_increment(ppu);
    }

    if (ppu->dot == 257)
    {
        copy_horizontal_bits(ppu);
    }

    ppu->dot++;
    if (ppu->dot >= 341)
    {
        ppu->dot = 0;
        ppu->scan_line++;

        if (ppu->scan_line == 241)
        {
            ppu->frame_ready = true;
            ppu->PPUSTATUS |= V_BLANK_BIT;
        }

        if (ppu->scan_line >= 262)
        {
            ppu->scan_line = 0;
        }
    }
}

uint8_t ppu_read(Ppu *ppu, uint16_t address)
{
    int selected_register = address & 0x0007;
    uint8_t data = 0;

    switch (selected_register)
    {
    case 0x0000:
        break;

    case 0x0001:
        break;
    case 0x0002:
        data = ppu->PPUSTATUS;
        ppu->write_toggle = 0;
        ppu->PPUSTATUS &= ~(V_BLANK_BIT);

        return data;
    case 0x0007:
        data = ppu->ppu_data_buffer;

        ppu->ppu_data_buffer = ppu_vram_read(ppu, ppu->vram_address);

        if (ppu->vram_address >= 0x3F00)
        {
            data = ppu->ppu_data_buffer;
            ppu->ppu_data_buffer = ppu_vram_read(ppu, ppu->vram_address - 0x1000);
        }
        uint16_t increment = (ppu->PPUCTRL & VRAM_INCREMENT ? 32 : 1);
        ppu->vram_address += increment;

        return data;
    };
    return data;
}

void ppu_write(Ppu *ppu, uint16_t address, uint8_t value)
{

    int selected_register = address & 0x0007;

    switch (selected_register)
    {
    case 0x0000:
        ppu->PPUCTRL = value;
        uint16_t datamask = (value & 0x03);
        ppu->temp_vram_address &= ~(0x0C00);
        ppu->temp_vram_address |= (datamask << 10);

        break;

    case 0x0001:
        ppu->PPUMASK = value;
        break;

    case 0x0005:

        if (ppu->write_toggle == 0)
        {
            uint16_t datamask_for_t = (value & 0xF8);
            uint8_t datamask_for_x = (value & 0x07);
            ppu->temp_vram_address &= ~(0x001F); // using ff instead of 3f clears all top bits including the unused 15th and sets 14th to 0
            ppu->temp_vram_address |= (datamask_for_t >> 3);
            ppu->fine_x = datamask_for_x;
            ppu->write_toggle = 1;
        }
        else
        { // Names from ppu scrolling page on nes dev
            uint16_t datamask_for_fgh = (value & 0x07);
            uint16_t datamask_for_ab = (value & 0xC0);  // seperated due to misreading doc
            uint16_t datamask_for_cde = (value & 0x38); // seperated due to misreading doc
            ppu->temp_vram_address &= ~(0x73E0);
            ppu->temp_vram_address |= (datamask_for_fgh << 12);
            ppu->temp_vram_address |= (datamask_for_ab << 2);
            ppu->temp_vram_address |= (datamask_for_cde << 2);
            ppu->write_toggle = 0;
        }

        break;

    case 0x0006:

        if (ppu->write_toggle == 0)
        {
            uint16_t datamask = (value & 0x3F);
            ppu->temp_vram_address &= ~(0xFF00); // using ff instead of 3f clears all top bits including the unused 15th and sets 14th to 0
            ppu->temp_vram_address |= (datamask << 8);
            ppu->write_toggle = 1;
        }
        else
        {
            ppu->temp_vram_address &= ~(0x00FF);
            ppu->temp_vram_address |= value;
            ppu->write_toggle = 0;

            ppu->vram_address = ppu->temp_vram_address;
            ppu->vram_address &= 0x7FFF;
        }

        break;

    case 0x0007:
        ppu_vram_write(ppu, ppu->vram_address, value);
        uint16_t increment = (ppu->PPUCTRL & VRAM_INCREMENT ? 32 : 1);
        ppu->vram_address += increment;

    default:
        break;
    };
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

// Debug function to print all four nametables to the standard text console
void print_nametables_to_console(Ppu *ppu)
{
    printf("\n┌─────────────────────────────────────────────────── NES NAMETABLES CONSOLE DUMP ─────────────────────────────────────────────────────┐\n");

    // Loop through rows of nametables stacked vertically (Top Screen Row vs Bottom Screen Row)
    for (int screen_row = 0; screen_row < 2; screen_row++)
    {
        // Each screen has 30 rows of tiles
        for (int tile_row = 0; tile_row < 30; tile_row++)
        {
            printf("│ "); // Left outer border

            // Loop through columns of nametables aligned horizontally (Left Screen vs Right Screen)
            for (int screen_col = 0; screen_col < 2; screen_col++)
            {
                // Calculate which of the 4 nametables we are looking at (0, 1, 2, or 3)
                int nt = (screen_row * 2) + screen_col;
                uint16_t nt_base_address = 0x2000 + (nt * 0x0400);

                // Each screen has 32 columns of tiles
                for (int tile_col = 0; tile_col < 32; tile_col++)
                {
                    // Fetch tile index from your mirroring-safe VRAM reader
                    uint16_t vram_addr = nt_base_address + (tile_row * 32) + tile_col;
                    uint8_t tile_index = ppu_vram_read(ppu, vram_addr);

                    // FIXED: Empty spaces must be 2 characters wide to match %02X formatting
                    if (tile_index == 0x00 || tile_index == 0x20)
                    {
                        printf("  ");
                    }
                    // ALTERNATIVE: If your font map maps ASCII directly (0x30='0', 0x41='A', etc.)
                    // and you'd rather read text than hex codes, uncomment the lines below:
                    /*
                    else if (tile_index >= 0x20 && tile_index <= 0x7E)
                    {
                        printf("%c ", tile_index); // Character + trailing space for alignment
                    }
                    */
                    else
                    {
                        printf("%02X", tile_index);
                    }
                }

                // Print a clean visual divider between the Left and Right screen layouts
                if (screen_col == 0)
                    printf(" │ ");
            }

            printf(" │\n"); // Right outer border and newline
        }

        // Print a visual horizontal divider between the Top and Bottom screen layouts
        if (screen_row == 0)
        {
            printf("├──────────────────────────────────────────────────────────────────┼──────────────────────────────────────────────────────────────────┤\n");
        }
    }
    printf("└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n\n");
}
