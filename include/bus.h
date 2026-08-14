#pragma once
#include <stdint.h>

typedef struct bus {
    uint8_t ram[2048];
} Bus;





void bus_init(Bus* bus);
