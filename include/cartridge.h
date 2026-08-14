#pragma once 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct cartridge
{
    uint8_t* prg_rom;
    uint8_t* chr_rom;

    uint8_t* save_data;

    bool hasBattery;



    size_t prg_size;
    size_t chr_size;

} Cartridge;



/**
 * Loads a rom from a file and populates the program rom data and character rom data
 * @param cartridge loads rom contents and fills the struct.
 * @param rom_name filename of rom to load
 */
bool cartridge_load(Cartridge* cartridge, const char* rom_name);
   


/**
 * Unloads the cartridge and frees the program rom data and character rom data
 * @param cartridge cartridge to be unloaded 
 */
void cartridge_unload(Cartridge* cartridge);
