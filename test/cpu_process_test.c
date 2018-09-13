#include "munit.h"
#include "../cpu.h"
#include "cpu_process_test.h"

/* 
 * LD bytes to registers 
 */

// Helper function for ld bytes
void assert_ld_byte(struct cpustate* cpu, unsigned char opcode, unsigned char* reg)
{
    unsigned char program_good[2] = { opcode, 0xAA };
    unsigned char program_bad[1] = { opcode };
    
    // Make sure bytes line up
    {
        init_cpu(cpu);
        int res = process_cpu(cpu, program_good, 2);
        munit_assert_int(res, ==, 0);
        munit_assert_int(*reg, ==, 0xAA);
        munit_assert_int((*cpu).PC, ==, 2);
    }   

    // Check for buffer overflow
    {
        init_cpu(cpu);
        int res = process_cpu(cpu, program_bad, 1);
        munit_assert_int(res, ==, -1);
        munit_assert_int(*reg, !=, 0xAA);
        munit_assert_int((*cpu).PC, ==, 0);
    }   
}

// Load to register A
MunitResult
    test_cpuprocess_3E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x3E, &(cpu.A));
}

// Load to register B
MunitResult
    test_cpuprocess_06(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x06, &(cpu.B));
}

// Load to register C
MunitResult
    test_cpuprocess_0E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x0E, &(cpu.C));
}

// Load to register D
MunitResult
    test_cpuprocess_16(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x16, &(cpu.D));
}

// Load to register E
MunitResult
    test_cpuprocess_1E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x1E, &(cpu.E));
}

// Load to register H
MunitResult
    test_cpuprocess_26(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x26, &(cpu.H));
}

// Load to register L
MunitResult
    test_cpuprocess_2E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_ld_byte(&cpu, 0x2E, &(cpu.L));
}

/* 
 * LXI - words to registers 
 */

// Helper function
void assert_lxi_word(struct cpustate* cpu, unsigned char opcode, uint16_t* reg)
{
    unsigned char program_good[3] = { opcode, 0xAA, 0xBB};
    unsigned char program_bad[1] = { opcode };

    // LXI instruction should set the SP to 0xBBAA
    // Make sure bytes line up
    {
        init_cpu(cpu);
        int res = process_cpu(cpu, program_good, 3);
        munit_assert_int(res, ==, 0);
        munit_assert_int(*reg, ==, 0xBBAA);
        munit_assert_int((*cpu).PC, ==, 3);
    }   

    // LXI instruction should fail, will overflow the buffer
    {
        init_cpu(cpu);
        (*reg) = 0; // We're setting it to zero since stack pointer isnt 0 by default
        int res = process_cpu(cpu, program_bad, 1);
        munit_assert_int(res, ==, -1);
        munit_assert_int(*reg, !=, 0xBBAA);
        munit_assert_int((*cpu).PC, ==, 0);
    }   
}

// Load word  to BC
MunitResult
    test_cpuprocess_01(const MunitParameter params[], void* fixture)
{
    // Setup CPU
    struct cpustate cpu;
    assert_lxi_word(&cpu, 0x01, &(cpu.BC));
}

// Load word  to DE
MunitResult
    test_cpuprocess_11(const MunitParameter params[], void* fixture)
{
    // Setup CPU
    struct cpustate cpu;
    assert_lxi_word(&cpu, 0x11, &(cpu.DE));
}

// Load word  to HL
MunitResult
    test_cpuprocess_21(const MunitParameter params[], void* fixture)
{
    // Setup CPU
    struct cpustate cpu;
    assert_lxi_word(&cpu, 0x21, &(cpu.HL));
}

// Load word to SP
MunitResult
    test_cpuprocess_31(const MunitParameter params[], void* fixture)
{
    // Setup CPU
    struct cpustate cpu;
    assert_lxi_word(&cpu, 0x31, &(cpu.SP));
}

// JMP to word
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


/*
 * CALL - call function at address
 */

// Calls a function at an address
MunitResult
    test_cpuprocess_CD(const MunitParameter params[], void* fixture)
{
    // Test functionality of CALL
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        unsigned char program[0xFFFF] = { 0x00, 0xCD, 0xAA, 0xFF };
        process_cpu(&cpu, program, 0xFFFF); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, 0xFFFF);
        munit_assert_int(res, ==, 0);   // Call should succeed
        munit_assert_int(cpu.SP, ==, STACK_START - 2); // Stack pointer should decrease by two
        munit_assert_int(cpu.PC, ==, 0xFFAA); // Current PC should point to new address
        munit_assert_int(program[cpu.SP - 2], ==, 0x01); // Stack - 2 should hold hi of return address
        munit_assert_int(program[cpu.SP - 1], ==, 0x00); // Stack - 2 should hold lo of return address
    }

    // Do not allow stack to underflow
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        cpu.SP = 0x00;
        unsigned char program[0xFFFF] = { 0x00, 0xCD, 0xAA, 0xFF };
        process_cpu(&cpu, program, 0xFFFF); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, 0xFFFF);
        munit_assert_int(res, ==, -1);   // Call should fail
        munit_assert_int(cpu.SP, ==, STACK_START); // Stack pointer should stay the same
    }

    // Do not jump out of bounds
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        cpu.SP = 0x00;
        unsigned char program[4] = { 0x00, 0xCD, 0xAA, 0xFF };
        process_cpu(&cpu, program, 4); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, 4);
        munit_assert_int(res, ==, -1);   // Call should fail
    }
}
