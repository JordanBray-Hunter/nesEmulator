#include "cpu.h"
#include "opcodes.h"
#include "stdio.h"

void update_zn_flags(Cpu *cpu, uint8_t value)
{

    cpu->p &= ~(Z_BIT | N_BIT);

    cpu->p |= (value & N_BIT);

    cpu->p |= (value == 0 ? Z_BIT : 0);
}

void update_alu_flags(Cpu *cpu, uint8_t accumulator, uint8_t value, uint16_t result)
{

    cpu->p &= ~(C_BIT | Z_BIT | V_BIT | N_BIT);
    cpu->p |= (result > 0xFF ? C_BIT : 0);

    cpu->p |= ((uint8_t)result == 0 ? Z_BIT : 0) | ((uint8_t)result & N_BIT);

    cpu->p |= ((((uint8_t)result ^ accumulator) & ((uint8_t)result ^ value) & 0x80) >> 1);
}

void compare_flags(Cpu *cpu, uint8_t value1, uint8_t value2)
{

    uint16_t result = value1 + (uint8_t)(~value2) + 1;

    cpu->p &= ~(C_BIT | Z_BIT | N_BIT);
    cpu->p |= (result > 0xFF ? C_BIT : 0);
    cpu->p |= ((uint8_t)result == 0 ? Z_BIT : 0);
    cpu->p |= ((uint8_t)result & N_BIT);
}

void cpu_reset(Cpu *cpu)
{
    cpu->sp -= 3;
    uint8_t low_byte = bus_read(cpu->bus, RESET_VECTOR_ADDRESS_LOW);
    uint8_t high_byte = bus_read(cpu->bus, RESET_VECTOR_ADDRESS_HIGH);

    cpu->pc = ((high_byte << 8) | low_byte);
    cpu->p = cpu->p | I_DISABLE_BIT;
}

void cpu_init(Cpu *cpu, Bus *bus)
{

    cpu->bus = bus;
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->cycles_remaining = 0;

    cpu->sp = 0xFD;
    uint8_t low_byte = bus_read(cpu->bus, RESET_VECTOR_ADDRESS_LOW);
    uint8_t high_byte = bus_read(cpu->bus, RESET_VECTOR_ADDRESS_HIGH);

    cpu->pc = ((high_byte << 8) | low_byte);
    cpu->p = 0;
    cpu->p = cpu->p | I_DISABLE_BIT | U_BIT;
}

void cpu_clock(Cpu *cpu)
{
    // printf("cycles remaining: %d \n ", cpu->cycles_remaining);

    if (cpu->cycles_remaining != 0)
    {
        cpu->cycles_remaining -= 1;
        return;
    }

    if (cpu->nmi_waiting)
    {
        bus_write(cpu->bus, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0xFF);
        cpu->sp--;
        bus_write(cpu->bus, 0x0100 + cpu->sp, cpu->pc & 0xFF);
        cpu->sp--;

        uint8_t status_to_push = (cpu->p | 0x20) & ~0x10;
        bus_write(cpu->bus, 0x0100 + cpu->sp, status_to_push);
        cpu->sp--;

        cpu->p |= I_DISABLE_BIT;

        uint8_t low = bus_read(cpu->bus, 0xFFFA);
        uint8_t high = bus_read(cpu->bus, 0xFFFB);
        cpu->pc = ((uint16_t)high << 8) | low;

        cpu->nmi_waiting = false;
        cpu->cycles_remaining = 6;
        return;
    }

    uint8_t opcode = bus_read(cpu->bus, cpu->pc++);

    // printf("current opcode: %d \n ", opcode);
    Instruction instruction = opcodes[opcode];

    if (instruction.opcode_func == NULL)
    {
        printf("UNIMPLEMENTED OPCODE: $%02X at PC=$%04X\n", opcode, cpu->pc - 1);
        exit(1);
    }

    cpu->cycles_remaining = instruction.cycles - 1;
    instruction.opcode_func(cpu, instruction.addr_func);

    return;
}