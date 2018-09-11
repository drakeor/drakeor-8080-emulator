#include "munit.h"
#include "../cpu.h"
#include "cpu_test.h"

void assert_state_zero(struct cpustate* state) {
    munit_assert_int(state->A, ==, 0);
    munit_assert_int(state->BC, ==, 0);
    munit_assert_int(state->DE, ==, 0);
    munit_assert_int(state->HL, ==, 0);
    munit_assert_int(state->SP, ==, STACK_START);
    munit_assert_int(state->PC, ==, PROGRAM_START);
    munit_assert_int(state->PSW, ==, 0);
}

MunitResult
    test_initcpu(const MunitParameter params[], void* fixture) {
        struct cpustate cpu;
        init_cpu(&cpu);
        assert_state_zero(&cpu); 
        return MUNIT_OK;
}

MunitResult
    test_initcpu_reset(const MunitParameter params[], void* fixture) {
        struct cpustate cpu;
        init_cpu(&cpu);
        assert_state_zero(&cpu); 
        
        cpu.A = 0xFF;
        cpu.BC = 0xFFFF;
        cpu.DE = 0xDEAD;
        cpu.HL = 0xDEAD;
        cpu.SP = 0xFEFE;
        cpu.PC = 0x1010;
        cpu.PSW = 0xDE;

        init_cpu(&cpu);
        assert_state_zero(&cpu);

        return MUNIT_OK;
}
