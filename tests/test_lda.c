#include <criterion/criterion.h>
#include "test_helpers.h"
#include "opcodes.h"
#include <stdio.h>

static TestSystem sys;


void setup(void) {
    make_test_system(&sys);
}

void teardown(void) {
    free_test_system(&sys);
}

TestSuite(lda, .init = setup, .fini = teardown);



Test(lda, immediate) {
    uint8_t program[] = { 0xA9, 0x42 }; // LDA #$42
    load_program(&sys, 0x8000, program, sizeof(program));
    sys.cpu.pc++;
    load_a_immediate(&sys.cpu);
    cr_assert_eq(sys.cpu.a, 0x42);
}

Test(lda, zero_page) {
    uint8_t program[] = { 0xA5, 0x10 }; 
    load_program(&sys, 0x8000, program, sizeof(program));
    sys.cpu.pc++;
    bus_write(&sys.bus, 0x10, 0x42);
    load_a_zp(&sys.cpu);
    printf("cpu.a = 0x%02X, cpu.pc = 0x%04X\n", sys.cpu.a, sys.cpu.pc);

    cr_assert_eq(sys.cpu.a, 0x42);
}

Test(lda, zero_page_x) {
    uint8_t program[] = { 0xB5, 0x10 }; 
    load_program(&sys, 0x8000, program, sizeof(program));
    sys.cpu.pc++;
    sys.cpu.x = 1;
    bus_write(&sys.bus, 0x11, 0x42);
    load_a_zp_x(&sys.cpu);
    printf("cpu.a = 0x%02X, cpu.pc = 0x%04X\n", sys.cpu.a, sys.cpu.pc);

    cr_assert_eq(sys.cpu.a, 0x42);
}














