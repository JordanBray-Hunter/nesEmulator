#include "bus.h"
#include "stdio.h"
#include <string.h>


void bus_init(Bus* bus, Cartridge* cartridge){

    bus->cartridge = cartridge;
    memset(bus->ram,0,sizeof(bus->ram));

}

