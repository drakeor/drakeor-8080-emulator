#include "munit.h"
#include "../cpu.h"
#include "cpu_process_test.h"

#define TEST_LD_BYTE(x, y) {  \
    init_cpu(&cpu); unsigned char program[2] = { x, 0xAA }; \
    munit_assert_int(res, ==, 0); munit_assert_int(y, ==, 0xAA); } \
    { init_cpu(&cpu); unsigned char program[1] = { x }; \
    munit_assert_int(res, ==, -1); munit_assert_int(y, ==, 0); };

// Helper function for ld bytes
void assert_ld_byte(struct cpustate* cpu, char opcode, char* reg)
{
    unsigned char program_good[2] = { opcode, 0xAA };
    unsigned char program_bad[1] = { opcode };
    
    // Make sure bytes line up
    {
        init_cpu(cpu);
        int res = process_cpu(cpu, program_good, 2);
        munit_assert_int(res, ==, 0);
        munit_assert_int(*reg, ==, 0xAA);
    }   

    // Check for buffer overflow
    {
        init_cpu(cpu);
        int res = process_cpu(cpu, program_bad, 1);
        munit_assert_int(res, ==, 0);
        munit_assert_int(*reg, ==, 0xAA);
    }   
}

MunitResult
    test_cpuprocess_3E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x3E, &(cpu.A));
}


MunitResult
    test_cpuprocess_06(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x06, &(cpu.B));
}


MunitResult
    test_cpuprocess_0E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x0E, &(cpu.C));
}

MunitResult
    test_cpuprocess_16(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x16, &(cpu.D));
}

MunitResult
    test_cpuprocess_1E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x1E, &(cpu.E));
}

MunitResult
    test_cpuprocess_26(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x26, &(cpu.H));
}

MunitResult
    test_cpuprocess_2E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x2E, &(cpu.L));
}

MunitResult
    test_cpuprocess_31(const MunitParameter params[], void* fixture)
{
    
    // Setup CPU
    struct cpustate cpu;

    // LXI instruction should set the SP to 0xBBAA
    {
        init_cpu(&cpu);
        unsigned char program[3] = { 0x31, 0xAA, 0xBB };
        int res = process_cpu(&cpu, program, 3);
        munit_assert_int(res, ==, 0);
        munit_assert_int(cpu.SP, ==, 0xBBAA);
    }

    // LXI instruction should fail, will overflow the buffer
    {
        init_cpu(&cpu);
        unsigned char program[2] = { 0x31, 0x01 };
        int res = process_cpu(&cpu, program, 2);
        munit_assert_int(res, ==, -1);
        munit_assert_int(cpu.SP, ==, STACK_START);
    }
}

MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture)
{    
    // Setup CPU
    struct cpustate cpu;

    // JMP instruction should set the PC to 0x0002
    {
        init_cpu(&cpu);
        unsigned char program[3] = { 0xC3, 0x02, 0x00 };
        int res = process_cpu(&cpu, program, 3);
        munit_assert_int(res, ==, 0);
        munit_assert_int(cpu.PC, ==, 0x0002);
    }

    // JMP instruction should set the PC to 0x88AA
    {
        init_cpu(&cpu);
        unsigned char program[0xFFFF] = { 0xC3, 0xAA, 0x88 };
        int res = process_cpu(&cpu, program, 0xFFFF);
        munit_assert_int(res, ==, 0);
        munit_assert_int(cpu.PC, ==, 0x88AA);
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
