#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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



typedef struct cartridge
{
    uint8_t *prg_rom;
    size_t prg_size;

    uint8_t *chr_rom;
    size_t chr_size;

    uint8_t *save_data;
    char* save_file_path;

    bool has_battery;

    uint8_t mapper_value;

    bool is_vertical;

} Cartridge;




/**
 * Loads a rom from a file and populates the program rom data and character rom data
 * @param cartridge loads rom contents and fills the struct.
 * @param rom_name filename of rom to load
 */
bool cartridge_load(Cartridge *cartridge, const char *rom_name);

/**
 * Unloads the cartridge and frees the program rom data and character rom data
 * @param cartridge cartridge to be unloaded
 */
void cartridge_unload(Cartridge *cartridge);







