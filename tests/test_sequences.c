#include <criterion/criterion.h>
#include "test_helpers.h"
#include "opcodes.h"
#include <stdio.h>

static TestSystem sys;

static void setup(void)
{
    make_test_system(&sys);
}

static void teardown(void)
{
    free_test_system(&sys);
}

TestSuite(test_sequences, .init = setup, .fini = teardown);



Test(test_sequences, lda_adc_sta_no_carry)
{
    uint8_t program[] = {0xA9, 0x42, 0x69, 0x08, 0x85, 0x05};
    load_program(&sys, 0x8000, program, sizeof(program));

    for(int i = 0; i < 7; i++){
        cpu_clock(&sys.cpu);
    }

    cr_assert_eq(bus_read(sys.cpu.bus,0x05), 0x4A);


}