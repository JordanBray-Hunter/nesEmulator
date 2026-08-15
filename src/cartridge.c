#include "cartridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_BYTE_COUNT 4
#define REMAINING_HEADERS_COUNT 12
#define TRAINER_SIZE 512
#define PROGRAM_ROM_BLOCK 16384
#define CHARACTER_ROM_BLOCK 8192
#define SAVE_DATA_SIZE 1024 * 8

// Flags
#define FLAGS6_TRAINER_BIT 0x04
#define FLAGS6_SAVEABLE 0x02
#define FLAGS6_MAPPER_LOW_MASK 0xF0
#define FLAGS7_MAPPER_HIGH_MASK 0xF0
#define FLAGS6_MIRRORING_BIT 0x01

bool cartridge_load(Cartridge *cartridge, const char *rom_name)
{

    FILE *rom = fopen(rom_name, "rb");

    if (rom == NULL)
    {
        printf("ERROR: File does not exist\n");
        return false;
    }

    uint8_t magic[MAGIC_BYTE_COUNT];
    uint8_t headers[REMAINING_HEADERS_COUNT];

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
    cartridge->prg_rom = calloc(cartridge->prg_size, sizeof(uint8_t));

    cartridge->chr_size = headers[1] * CHARACTER_ROM_BLOCK;
    cartridge->chr_rom = calloc(cartridge->chr_size, sizeof(uint8_t));

    if (headers[2] & FLAGS6_TRAINER_BIT)
    {
        fseek(rom, TRAINER_SIZE, SEEK_CUR);
    }
    if (headers[2] & FLAGS6_SAVEABLE)
    {
        cartridge->has_battery = true;
        cartridge->save_data = calloc(SAVE_DATA_SIZE, sizeof(char));
        char *extension = strrchr(rom_name, '.');
        int base_length = 0;
        if (extension == NULL)
        {
            base_length = strlen(rom_name);
        }
        else
        {
            base_length = extension - rom_name;
        }

        char sav[base_length + 5];
        memset(sav, '\0', sizeof(sav));
        memcpy(sav, rom_name, base_length);
        strcpy(sav + (base_length), ".sav");
        cartridge->save_file_path = malloc(base_length + 5);
        memcpy(cartridge->save_file_path, rom_name, base_length);
        strcpy(cartridge->save_file_path + base_length, ".sav");
        printf("INFO: Savefile: %s \n", sav);
        printf("INFO: Attempting to load save data\n");
        FILE *save_file = fopen(sav, "rb");
        if (save_file != NULL)
        {
            printf("INFO: File found\n");
            size_t bytes_read = fread(cartridge->save_data, sizeof(char), SAVE_DATA_SIZE, save_file);
            if (bytes_read < SAVE_DATA_SIZE)
            {
                fclose(save_file);
                fclose(rom);
                return false;
            }
        }
        else
        {
            printf("INFO: File not found\n");
            save_file = fopen(sav, "wb");
            fwrite(cartridge->save_data, sizeof(char), SAVE_DATA_SIZE, save_file);
            fclose(save_file);
        }
    }

    uint8_t lower_mapper = headers[2] & FLAGS6_MAPPER_LOW_MASK;
    uint8_t upper_map = headers[3] & FLAGS7_MAPPER_HIGH_MASK;
    cartridge->mapper_value = upper_map | (lower_mapper >> 4);

    cartridge->is_vertical = ((headers[2] & FLAGS6_MIRRORING_BIT) == 0);

    // Load program rom data and character rom data

    size_t prg_bytes_loaded = fread(cartridge->prg_rom, sizeof(uint8_t), cartridge->prg_size, rom);
    if (prg_bytes_loaded != cartridge->prg_size)
    {
        fclose(rom);
        printf("ERROR: Could not load all of the program rom data\n");
        return false;
    }
    size_t chr_bytes_loaded = fread(cartridge->chr_rom, sizeof(uint8_t), cartridge->chr_size, rom);
    if (chr_bytes_loaded != cartridge->chr_size)
    {
        fclose(rom);
        printf("ERROR: Could not load all of the character rom data\n");
        return false;
    }

    fclose(rom);
    return true;
}

void cartridge_unload(Cartridge *cartridge)
{

    free(cartridge->chr_rom);
    free(cartridge->prg_rom);
    free(cartridge->save_data);
}