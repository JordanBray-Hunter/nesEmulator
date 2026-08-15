#pragma once
#include <stdint.h>
#include "cartridge.h"

#define RAM_SIZE 2048

typedef struct bus {
    uint8_t ram[RAM_SIZE];
    Cartridge* cartridge;


} Bus;





void bus_init(Bus* bus, Cartridge* cartridge);
