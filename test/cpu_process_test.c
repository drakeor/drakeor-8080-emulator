#include "munit.h"
#include "../cpu.h"
#include "cpu_process_test.h"

MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture)
{    
    // Setup CPU
    struct cpustate cpu;

    // JMP instruction should set the PC to 0x0001
    {
        init_cpu(&cpu);
        unsigned char program[3] = { 0xC3, 0x01, 0x00 };
        int res = process_cpu(&cpu, program, 3);
        munit_assert_int(res, ==, 0);
        munit_assert_int(cpu.PC, ==, 0x0001);
    }

    // JMP instruction should fail, will overflow the buffer
    {
        init_cpu(&cpu);
        unsigned char program[2] = { 0xC3, 0x01 };
        int res = process_cpu(&cpu, program, 2);
        munit_assert_int(res, ==, -1);
        munit_assert_int(cpu.PC, ==, 0x0000);
    }

    // JMP instruction should fail, jumping to protected memory
    {
        init_cpu(&cpu);
        unsigned char program[3] = { 0xC3, 0xFF, 0xFF };
        int res = process_cpu(&cpu, program, 3);
        munit_assert_int(res, ==, -1);
        munit_assert_int(cpu.PC, ==, 0x0000);
    }

    return MUNIT_OK;
}
