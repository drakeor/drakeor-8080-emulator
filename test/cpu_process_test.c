#include "munit.h"
#include "../cpu.h"
#include "cpu_process_test.h"


/*
 *  Test helper helper functions
 */

// Makes sure failure occurs when it jumps outside memory bounds
void test_overflow_byte(struct cpustate* cpu, uint8_t opcode)
{
    init_cpu(cpu);
    uint8_t program_bad[1] = { opcode };
    int res = process_cpu(cpu, program_bad, 1);
    munit_assert_int(res, ==, -1);
}

// Makes sure failure occurs when it jumps outside memory bounds
void test_overflow_word(struct cpustate* cpu, uint8_t opcode)
{
    init_cpu(cpu);
    uint8_t program_bad[2] = { opcode, 0x00 };
    int res = process_cpu(cpu, program_bad, 2);
    munit_assert_int(res, ==, -1);
}

/*
 *  Test helper functions
 */

// Helper function for LXI words
void assert_lxi_word(struct cpustate* cpu, uint8_t opcode, uint16_t* reg)
{
    uint8_t program_good[3] = { opcode, TEST_MEMORY_RAM_L, TEST_MEMORY_RAM_H};
    uint8_t program_bad[1] = { opcode };

    // LXI instruction should set the SP to 0xBBAA
    // Make sure bytes line up
    {
        init_cpu(cpu);
        int res = process_cpu(cpu, program_good, 3);
        munit_assert_int(res, ==, 0);
        munit_assert_int(*reg, ==, TEST_MEMORY_RAM_HL);
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

// Helper function for LD bytes
void assert_ld_byte(struct cpustate* cpu, uint8_t opcode, uint8_t* reg)
{
    uint8_t program_good[2] = { opcode, 0xAA };
    
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
        test_overflow_byte(cpu, opcode);
        munit_assert_int(*reg, !=, 0xAA);
        munit_assert_int((*cpu).PC, ==, 0);
    }   
}

// Helper function for call functions
// We do not init the CPU in this
void assert_call_function_true(struct cpustate* cpu, uint8_t opcode)
{
    // Call if that flag is true
    uint8_t program[MEMORY_SIZE] = { opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
    int res = process_cpu(cpu, program, MEMORY_SIZE);
    munit_assert_int(res, ==, 0);   // Call should succeed
    munit_assert_int(cpu->SP, ==, STACK_START - 2); // Stack pointer should decrease by two
    munit_assert_int(cpu->PC, ==, TEST_MEMORY_ROM_HL); // Current PC should point to new address
}

// Helper function for call functions
// We do not init the CPU in this
void assert_call_function_false(struct cpustate* cpu, uint8_t opcode)
{
    // Do not call if that flag is false
    uint8_t program[MEMORY_SIZE] = { opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
    int res = process_cpu(cpu, program, MEMORY_SIZE);
    munit_assert_int(res, ==, 0);   // Call should succeed
    munit_assert_int(cpu->SP, ==, STACK_START); // Stack pointer shouldn't decrease by two
    munit_assert_int(cpu->PC, ==, 0x03); // Current PC should point to new address
}

/* 
 * LD bytes to registers 
 */

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
        uint8_t program[3] = { 0xC3, 0x02, 0x00 };
        int res = process_cpu(&cpu, program, 3);
        munit_assert_int(res, ==, 0);
        munit_assert_int(cpu.PC, ==, 0x0002);
    }

    // JMP instruction should set the PC
    {
        init_cpu(&cpu);
        uint8_t program[MEMORY_SIZE] = { 0xC3, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
        int res = process_cpu(&cpu, program, MEMORY_SIZE);
        munit_assert_int(res, ==, 0);
        munit_assert_int(cpu.PC, ==, TEST_MEMORY_ROM_HL);
    }

    // Ensure there is no overflow
    test_overflow_word(&cpu, 0xC3);

    // JMP instruction should fail, jumping to out of bounds
    {
        init_cpu(&cpu);
        uint8_t program[3] = { 0xC3, TEST_MEMORY_OOB_L, TEST_MEMORY_OOB_H };
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

    // Ensure there is no overflow
    {
        struct cpustate cpu;
        test_overflow_word(&cpu, 0xCD);
    }

    // Test functionality of CALL
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        uint8_t program[MEMORY_SIZE] = { 0x00, 0xCD, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
        process_cpu(&cpu, program, MEMORY_SIZE); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, MEMORY_SIZE);
        munit_assert_int(res, ==, 0);   // Call should succeed
        munit_assert_int(cpu.SP, ==, STACK_START - 2); // Stack pointer should decrease by two
        munit_assert_int(cpu.PC, ==, TEST_MEMORY_ROM_HL); // Current PC should point to new address
        printf("Memory: 0x%02X 0x%02X\n", program[cpu.SP + 1], program[cpu.SP + 2]);
        munit_assert_int(program[cpu.SP], ==, 0x00); // Stack should hold hi of return address
        munit_assert_int(program[cpu.SP + 1], ==, 0x01); // Stack + 1 should hold lo of return address
    }

    // Do not allow stack to underflow
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        cpu.SP = 0x00;
        uint8_t program[MEMORY_SIZE] = { 0x00, 0xCD, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
        process_cpu(&cpu, program, MEMORY_SIZE); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, MEMORY_SIZE);
        munit_assert_int(res, ==, -1);   // Call should fail
        munit_assert_int(cpu.SP, ==, 0x00); // Stack pointer should stay the same
    }

    // Do not allow stack to write into ROM
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        cpu.SP = TEST_MEMORY_ROM_HL;
        uint8_t program[MEMORY_SIZE] = { 0x00, 0xCD, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
        process_cpu(&cpu, program, MEMORY_SIZE); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, MEMORY_SIZE);
        munit_assert_int(res, ==, -1);   // Call should fail
        munit_assert_int(cpu.SP, ==, TEST_MEMORY_ROM_HL); // Stack pointer should stay the same
    }

    // Do not jump out of bounds
    {
        struct cpustate cpu;
        init_cpu(&cpu);
        uint8_t program[4] = { 0x00, 0xCD, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
        process_cpu(&cpu, program, 4); // Process twice to get rid of NOP
        int res = process_cpu(&cpu, program, 4);
        munit_assert_int(res, ==, -1);   // Call should fail
    }

    return MUNIT_OK;
}


MunitResult
    test_cpuprocess_C4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xC4);

    init_cpu(&cpu);
    cpu.FLAGS.Z = 1;
    assert_call_function_true(&cpu, 0xC4);

    init_cpu(&cpu);
    assert_call_function_false(&cpu, 0xC4);
}

MunitResult
    test_cpuprocess_CC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xCC);

    init_cpu(&cpu);
    assert_call_function_true(&cpu, 0xCC);

    init_cpu(&cpu);
    cpu.FLAGS.Z = 1;
    assert_call_function_false(&cpu, 0xCC);
}

MunitResult
    test_cpuprocess_D4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xD4);

    init_cpu(&cpu);
    assert_call_function_true(&cpu, 0xD4);

    init_cpu(&cpu);
    cpu.FLAGS.C = 1;
    assert_call_function_false(&cpu, 0xD4);
}

MunitResult
    test_cpuprocess_DC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xDC);

    init_cpu(&cpu);
    cpu.FLAGS.C = 1;
    assert_call_function_true(&cpu, 0xDC);

    init_cpu(&cpu);
    assert_call_function_false(&cpu, 0xDC);
}

MunitResult
    test_cpuprocess_E4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    
    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xE4);

    init_cpu(&cpu);
    assert_call_function_true(&cpu, 0xE4);

    init_cpu(&cpu);
    cpu.FLAGS.P = 1;
    assert_call_function_false(&cpu, 0xE4);
}

MunitResult
    test_cpuprocess_EC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xEC);

    init_cpu(&cpu);
    cpu.FLAGS.P = 1;
    assert_call_function_true(&cpu, 0xEC);

    init_cpu(&cpu);
    assert_call_function_false(&cpu, 0xEC);
}

MunitResult
    test_cpuprocess_F4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xF4);

    init_cpu(&cpu);
    assert_call_function_true(&cpu, 0xF4);

    init_cpu(&cpu);
    cpu.FLAGS.S = 1;
    assert_call_function_false(&cpu, 0xF4);
}

MunitResult
    test_cpuprocess_FC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    init_cpu(&cpu);
    test_overflow_word(&cpu, 0xFC);

    init_cpu(&cpu);
    cpu.FLAGS.S = 1;
    assert_call_function_true(&cpu, 0xFC);

    init_cpu(&cpu);
    assert_call_function_false(&cpu, 0xFC);
}
