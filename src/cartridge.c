#include "cartridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_BYTE_COUNT 4
#define REMAINING_HEADERS_COUNT 12
#define FLAGS6_TRAINER_BIT 0x04
#define FLAGS6_SAVEABLE 0x02
#define FLAGS6_MAPPER_LOW_MASK 0xF0
#define FLAGS7_MAPPER_HIGH_MASK 0xF0
#define TRAINER_SIZE 512
#define PROGRAM_ROM_BLOCK 16384
#define CHARACTER_ROM_BLOCK 8192


bool cartridge_load(Cartridge *cartridge, const char *rom_name)
{

    FILE *rom = fopen(rom_name, "rb");

    if (rom == NULL)
    {
        printf("ERROR: File does not exist\n");
        return false;
    }

    char magic[MAGIC_BYTE_COUNT];
    char headers[REMAINING_HEADERS_COUNT];

    fread(magic, sizeof(char), MAGIC_BYTE_COUNT, rom);
    if (memcmp(magic, "NES\x1A", MAGIC_BYTE_COUNT) != 0)
    {

        printf("ERROR: Magic number is incorrect\n");
        fclose(rom);
        return false;
    }
    else
    {
        printf("INFO: Magic number is correct\n");
    }
    fread(headers, sizeof(char), REMAINING_HEADERS_COUNT, rom);

    cartridge->prg_size = headers[0] * PROGRAM_ROM_BLOCK;
    cartridge->chr_size = headers[1] * CHARACTER_ROM_BLOCK;




}

void cartridge_unload(Cartridge *cartridge)
{

    free(cartridge->chr_rom);
    free(cartridge->prg_rom);
}