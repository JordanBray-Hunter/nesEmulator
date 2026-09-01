#include <criterion/criterion.h>
#include "test_helpers.h"
#include "opcodes.h"
#include <stdio.h>
#include "cpu.h"

static TestSystem sys;

static void setup(void)
{
    make_test_system(&sys);
}

static void teardown(void)
{
    free_test_system(&sys);
}

TestSuite(cmp, .init = setup, .fini = teardown);

Test(cmp, immediate_a_greater)
{
    uint8_t program[] = {0xC9, 0x20};
    sys.cpu.a = 0x22;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x22);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, C_BIT);
    CHECK_FLAG(N_BIT, 0);
}

Test(cmp, immediate_a_equal)
{
    uint8_t program[] = {0xC9, 0x22};
    sys.cpu.a = 0x22;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x22);

    CHECK_FLAG(Z_BIT, Z_BIT);
    CHECK_FLAG(C_BIT, C_BIT);
    CHECK_FLAG(N_BIT, 0);
}

Test(cmp, immediate_a_less)
{
    uint8_t program[] = {0xC9, 0x23};
    sys.cpu.a = 0x22;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x22);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, 0);
    CHECK_FLAG(N_BIT, N_BIT);
}

Test(cmp, zp)
{
    uint8_t program[] = {0xC5, 0x23};
    sys.cpu.a = 0x22;

    bus_write(sys.cpu.bus,0x23,0x10);
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x22);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, C_BIT);
    CHECK_FLAG(N_BIT, 0);
}

