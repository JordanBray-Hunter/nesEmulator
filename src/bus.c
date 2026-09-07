#include "bus.h"
#include "stdio.h"
#include <string.h>

void bus_init(Bus *bus, Cartridge *cartridge, Ppu *ppu, Controller *controller)
{

    bus->cartridge = cartridge;
    bus->controller = controller;
    memset(bus->ram, 0, sizeof(bus->ram));
    bus->ppu = ppu;
}

uint8_t bus_read(Bus *bus, uint16_t address)
{

    if (0x0000 <= address && address <= 0x1FFF)
    {
        return bus->ram[address % 0x0800];
    }
    if (0x2000 <= address && address <= 0x3FFF)
    {
        // TOOD: change to map to register
        return ppu_read(bus->ppu, address);
    }
    if (0x4000 <= address && address <= 0x4017)
    {
        // TODO: Map to NES APU AND IO
        if (address == 0x4016)
        {
            return controller_read(bus->controller);
        }

        return 0;
    }
    if (0x4018 <= address && address <= 0x401F)
    {
        // USUALLY UNUSED, will leave blank for now.
        return 0;
    }
    if (0x4020 <= address && address <= 0xFFFF)
    {
        // TODO: Mapt to cartridge, based off mapper.
        //

        uint16_t mapped_index = (address - 0x8000) % bus->cartridge->prg_size;
        return bus->cartridge->prg_rom[mapped_index];
    }

    

    return 0;
}

void bus_write(Bus *bus, uint16_t address, uint8_t value)
{

    if (0x0000 <= address && address <= 0x1FFF)
    {
        bus->ram[address % 0x0800] = value;
    }
    if (0x2000 <= address && address <= 0x3FFF)
    {
        ppu_write(bus->ppu, address, value);
    }
    if (0x4000 <= address && address <= 0x4017)
    {
        // TODO: Map to NES APU AND IO
        if (address == 0x4016)
        {
            controller_write(bus->controller, value);
        }
        return;
    }
    if (0x4018 <= address && address <= 0x401F)
    {
        // USUALLY UNUSED, will leave blank for now.
        return;
    }
    if (0x4020 <= address && address <= 0xFFFF)
    {
        // TODO: Mapt to cartridge, based off mapper.

        bus->cartridge->prg_rom[address - 0x8000];
    }

    return;
}
