#include "bus.h"
#include "stdio.h"
#include <string.h>


void bus_init(Bus* bus, Cartridge* cartridge,Ppu *ppu){

    bus->cartridge = cartridge;
    memset(bus->ram,0,sizeof(bus->ram));
    bus->ppu = ppu;
}

uint8_t bus_read(Bus *bus, uint16_t address){

    if(0x0000  <= address  && address  <= 0x1FFF){
        return bus->ram[address % 0x0800];
    }
    if(0x2000 <= address && address <= 0x3FFF){
        //TOOD: change to map to register
        return 0;
    }
    if(0x4000 <= address && address <= 0x4017){
        //TODO: Map to NES APU AND IO
        return 0;
    }
    if(0x4018 <= address && address <= 0x401F){
        //USUALLY UNUSED, will leave blank for now.
        return 0;
    }
    if(0x4020 <= address && address <= 0xFFFF){
        //TODO: Mapt to cartridge, based off mapper. 
        //


       return bus->cartridge->prg_rom[address - 0x8000];
    }


    return 0;


}

