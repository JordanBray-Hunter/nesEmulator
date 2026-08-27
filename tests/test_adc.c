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

TestSuite(adc, .init = setup, .fini = teardown);

Test(adc, immediate_no_carry)
{
    uint8_t program[] = {0x69, 0x20};
    sys.cpu.a = 0x22;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x42);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, 0);
    CHECK_FLAG(V_BIT, 0);
    CHECK_FLAG(N_BIT, 0);
}

Test(adc, immediate_with_carry)
{
    uint8_t program[] = {0x69, 0x20};
    sys.cpu.a = 0x22;
    set_carry(&sys.cpu);
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x43);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, 0);
    CHECK_FLAG(V_BIT, 0);
    CHECK_FLAG(N_BIT, 0);
}

Test(adc, zero_flag)
{
    uint8_t program[] = {0x69, 0x0};
    sys.cpu.a = 0;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0);
    CHECK_FLAG(Z_BIT, Z_BIT);
}

Test(adc, carry_out)
{
    uint8_t program[] = {0x69, 0x01};
    sys.cpu.a = 0xFF;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0);
    CHECK_FLAG(C_BIT, C_BIT);
}


Test(adc, overflow)
{
    uint8_t program[] = {0x69, 0x7F};
    sys.cpu.a = 0x01;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x80);
    CHECK_FLAG(V_BIT, V_BIT);
    CHECK_FLAG(N_BIT,N_BIT);
}