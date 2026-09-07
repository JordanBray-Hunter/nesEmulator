#include <raylib.h>
#include "cartridge.h"
#include "bus.h"
#include <stdio.h>
#include "cpu.h"
#include "controller.h"


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        printf("Error: please spesify rom file\n");
        return 1;
    }

    const int screenWidth = 256;
    const int screenHeight = 240;
    const int scale = 4;
    InitWindow(screenWidth * scale, screenHeight * scale, "EMULATOR");

    Cartridge cartridge;
    Bus bus;
    Ppu ppu;
    Cpu cpu;
    Controller controller;

    bool loaded = cartridge_load(&cartridge, argv[1]);

    printf("=== PRG VECTOR INTEGRITY CHECK ===\n");
    printf("PRG Size Loaded: %d bytes\n", cartridge.prg_size);
    printf("Byte at internal PRG index [32764] ($FFFC): 0x%02X\n", cartridge.prg_rom[32764]);
    printf("Byte at internal PRG index [32765] ($FFFD): 0x%02X\n", cartridge.prg_rom[32765]);

    bool show_tiles = false;

    if (!loaded)
    {
        printf("ERROR: Failed to load rom");
        return 1;
    }

    bus_init(&bus, &cartridge, &ppu,&controller);
    cpu_init(&cpu, &bus);
    ppu_init(&ppu, &cartridge, &cpu);

    SetTargetFPS(60);

    RenderTexture2D chr_texture = LoadRenderTexture(256, 128);

    draw_chrs_to_texture(&chr_texture, cartridge.chr_rom);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_P))
        {
            show_tiles = !show_tiles;
        }
        if (IsKeyPressed(KEY_O))
        {
            ppu.PPUMASK ^= BG_ENABLE_BIT;
            ppu.PPUSTATUS |= V_BLANK_BIT;
            
        }
        if (IsKeyPressed(KEY_D))
        {
            print_nametables_to_console(&ppu);
        }
        if (IsKeyPressed(KEY_C))
        {
            printf("=== ppu_vram_read CHR sanity check ===\n");
            for (int tile = 0; tile < 10; tile++)
            {
                uint16_t addr_lsb = (tile << 4);
                uint16_t addr_msb = (tile << 4) | 0x8;

                uint8_t direct_lsb = cartridge.chr_rom[addr_lsb]; // known-good, straight from your CHR viewer's method
                uint8_t direct_msb = cartridge.chr_rom[addr_msb];

                uint8_t via_read_lsb = ppu_vram_read(&ppu, addr_lsb);
                uint8_t via_read_msb = ppu_vram_read(&ppu, addr_msb);

                printf("tile %d: direct_lsb=%02X via_read_lsb=%02X | direct_msb=%02X via_read_msb=%02X %s\n",
                       tile, direct_lsb, via_read_lsb, direct_msb, via_read_msb,
                       (direct_lsb == via_read_lsb && direct_msb == via_read_msb) ? "OK" : "MISMATCH");
            }
        }

        while (!ppu.frame_ready)
        {
            controller_update(&controller);
            cpu_clock(&cpu);
            ppu_clock(&ppu);
            ppu_clock(&ppu);
            ppu_clock(&ppu);


        }

        UpdateTexture(ppu.gameTexture.texture, ppu.pixels);
        BeginDrawing();
        ClearBackground(WHITE);
        ppu.frame_ready = false;

        if (show_tiles)
        {
            Rectangle source = {0, 0, (float)chr_texture.texture.width, -(float)chr_texture.texture.height};
            Rectangle dest = {0, 0, (float)chr_texture.texture.width * scale, (float)chr_texture.texture.height * scale};
            Vector2 origin = {0, 0};

            DrawTexturePro(chr_texture.texture, source, dest, origin, 0.0f, WHITE);
        }
        else
        {
            Rectangle source = {0, 0, (float)ppu.gameTexture.texture.width, (float)ppu.gameTexture.texture.height};
            Rectangle dest = {0, 0, (float)ppu.gameTexture.texture.width * scale, (float)ppu.gameTexture.texture.height * scale};
            Vector2 origin = {0, 0};

            DrawTexturePro(ppu.gameTexture.texture, source, dest, origin, 0.0f, WHITE);
        }

        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    cartridge_unload(&cartridge);

    return 0;
}