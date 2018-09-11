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

// Ensures the cpu registers start at the proper values
MunitResult
    test_initcpu(const MunitParameter params[], void* fixture) {
        struct cpustate cpu;
        init_cpu(&cpu);
        assert_state_zero(&cpu); 
        return MUNIT_OK;
}

// Ensures calling init_cpu again resets it
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

// Make sure the unions work as intended
MunitResult
    test_initcpu_registers(const MunitParameter params[], void* fixture) {
        struct cpustate cpu;
        init_cpu(&cpu);
        assert_state_zero(&cpu);
        
        cpu.A = 0xFF;
        munit_assert_int(cpu.A, ==, 0xFF);

        cpu.B = 0xFF;
        munit_assert_int(cpu.BC, ==, 0xFF00);

        cpu.B = 0xAA;
        cpu.C = 0xFF;
        munit_assert_int(cpu.BC, ==, 0xAAFF);

        cpu.D = 0xAA;
        munit_assert_int(cpu.DE, ==, 0xAA00);

        cpu.L = 0xBB;
        munit_assert_int(cpu.HL, ==, 0x00BB);
        
        cpu.SP = 0xAAAA;
        munit_assert_int(cpu.SP, ==, 0xAAAA);

        cpu.PC = 0x1234;
        munit_assert_int(cpu.PC, ==, 0x1234); 

        cpu.FLAGS.S = 1;
        cpu.FLAGS.Z = 1;
        cpu.FLAGS.AC = 1;
        cpu.FLAGS.C = 1;
        cpu.FLAGS.P = 1;
        munit_assert_int(cpu.PSW, ==, 0b11010101); 
}
