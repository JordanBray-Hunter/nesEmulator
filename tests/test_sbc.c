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

TestSuite(sbc, .init = setup, .fini = teardown);


Test(sbc, immediate_no_carry)
{
    uint8_t program[] = {0xE9, 0x0A};
    sys.cpu.a = 20;
    set_carry(&sys.cpu);
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);


    cr_assert_eq(sys.cpu.a, 10);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, C_BIT);
    CHECK_FLAG(V_BIT, 0);
    CHECK_FLAG(N_BIT, 0);
}

Test(sbc, immediate_with_carry)
{
    //Carry not set as its inverted with subtraction
    //No carry bit means borrow 
    uint8_t program[] = {0xE9, 0x0A};
    sys.cpu.a = 20;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);

    cr_assert_eq(sys.cpu.a, 9);

    CHECK_FLAG(Z_BIT, 0);
    CHECK_FLAG(C_BIT, C_BIT);
    CHECK_FLAG(V_BIT, 0);
    CHECK_FLAG(N_BIT, 0);
}


Test(sbc, zero_flag)
{
    uint8_t program[] = {0xE9, 0x20};
    sys.cpu.a = 0x20;
    set_carry(&sys.cpu);
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0);
    CHECK_FLAG(Z_BIT, Z_BIT);
}

// Test(adc, carry_out)
// {
//     uint8_t program[] = {0x69, 0x01};
//     sys.cpu.a = 0xFF;
//     load_program(&sys, 0x8000, program, sizeof(program));
//     cpu_clock(&sys.cpu);
//     cr_assert_eq(sys.cpu.a, 0);
//     CHECK_FLAG(C_BIT, C_BIT);
// }



Test(sbc, becommes_negitive)
{
    uint8_t program[] = {0xE9, 0x20};
    sys.cpu.a = 0x01;
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0xE0);
    
    CHECK_FLAG(V_BIT, 0);
    CHECK_FLAG(N_BIT,N_BIT);
}


Test(sbc, overflow)
{
    uint8_t program[] = {0xE9, 0x1};
    sys.cpu.a = 0x80;
    set_carry(&sys.cpu);
    load_program(&sys, 0x8000, program, sizeof(program));
    cpu_clock(&sys.cpu);

    cr_assert_eq(sys.cpu.a, 127);
    
    CHECK_FLAG(V_BIT, V_BIT);
    CHECK_FLAG(N_BIT,0);
}

