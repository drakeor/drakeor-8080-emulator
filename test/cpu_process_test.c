#include "munit.h"
#include "../cpu.h"
#include "cpu_process_test.h"


/*
 * Test helper macros.
 * We use a ton of macros to help trace munit back to the proper line numbers for failed tests.
 */

#define SETUP_TEST_1(c1) init_cpu(cpu); uint8_t program[MEMORY_SIZE] = {c1};
#define SETUP_TEST_2(c1, c2) init_cpu(cpu); uint8_t program[MEMORY_SIZE] = {c1, c2};
#define SETUP_TEST_3(c1, c2, c3) init_cpu(cpu); uint8_t program[MEMORY_SIZE] = {c1, c2, c3};
#define SETUP_TEST_4(c1, c2, c3, c4) init_cpu(cpu); uint8_t program[MEMORY_SIZE] = {c1, c2, c3, c4};

#define TEST_SUCCESS() { int res = process_cpu(cpu, program, MEMORY_SIZE); munit_assert_int(res, ==, 0); }
#define TEST_FAIL() { int res = process_cpu(cpu, program, MEMORY_SIZE); munit_assert_int(res, ==, -1); }

#define TEST_SUCCESS_OPCODE() TEST_SUCCESS(); munit_assert_int(cpu->PC, ==, 1); 
#define TEST_SUCCESS_BYTE() TEST_SUCCESS(); munit_assert_int(cpu->PC, ==, 2); 
#define TEST_SUCCESS_WORD() TEST_SUCCESS(); munit_assert_int(cpu->PC, ==, 3); 

#define TEST_FAIL_GENERIC() TEST_FAIL(); munit_assert_int(cpu->PC, ==, 0); 

#define SETUP_TEST_OVERFLOW_BYTE(x) init_cpu(cpu); uint8_t program[1] = { x }; int res = process_cpu(cpu, program, 1); munit_assert_int(res, ==, -1);
#define SETUP_TEST_OVERFLOW_WORD(x) init_cpu(cpu); uint8_t program[2] = { x, 0x00 }; int res = process_cpu(cpu, program, 2); munit_assert_int(res, ==, -1);

/*
 * Test helper functions
 */ 
uint8_t get_psw_flag_z() 
{
    struct cpustate flaggedcpu;
    init_cpu(&flaggedcpu);
    flaggedcpu.FLAGS.Z = 1;
    return flaggedcpu.PSW;
}

uint8_t get_psw_flag_ac() 
{
    struct cpustate flaggedcpu;
    init_cpu(&flaggedcpu);
    flaggedcpu.FLAGS.AC = 1;
    return flaggedcpu.PSW;
}

uint8_t get_psw_flag_c() 
{
    struct cpustate flaggedcpu;
    init_cpu(&flaggedcpu);
    flaggedcpu.FLAGS.C = 1;
    return flaggedcpu.PSW;
}

uint8_t get_psw_flag_p() 
{
    struct cpustate flaggedcpu;
    init_cpu(&flaggedcpu);
    flaggedcpu.FLAGS.P = 1;
    return flaggedcpu.PSW;
}

uint8_t get_psw_flag_s() 
{
    struct cpustate flaggedcpu;
    init_cpu(&flaggedcpu);
    flaggedcpu.FLAGS.S = 1;
    return flaggedcpu.PSW;
}

/* 
 * LD bytes to registers 
 */

// Helper function for LD bytes
void assert_ld_byte(struct cpustate* cpu, uint8_t opcode, uint8_t* reg)
{
    
    // Make sure bytes line up
    {
        SETUP_TEST_2(opcode, TEST_MEMORY_ROM_L);
        TEST_SUCCESS_BYTE();
        munit_assert_int(*reg, ==, TEST_MEMORY_ROM_L);
    }   

    // Check for buffer overflow
    {
        SETUP_TEST_OVERFLOW_BYTE(opcode);
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

// Helper function for LXI words
void assert_lxi_word(struct cpustate* cpu, uint8_t opcode, uint16_t* reg)
{

    // LXI instruction should set the SP to 0xBBAA
    // Make sure bytes line up
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_RAM_L, TEST_MEMORY_RAM_H);
        TEST_SUCCESS_WORD();
        munit_assert_int(*reg, ==, TEST_MEMORY_RAM_HL);
    }   

    // LXI instruction should fail, will overflow the buffer
    {
        SETUP_TEST_OVERFLOW_WORD(opcode);
        munit_assert_int(*reg, !=, 0xBBAA);
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

/*
 * JMPS to boundaries
 */

// JMP to word
void assert_jump_opcode(struct cpustate* cpu, uint8_t opcode)
{

    // JMP instruction should set the PC to 0x0002
    {
        SETUP_TEST_3(opcode, 0x02, 0x00);
        TEST_SUCCESS();
        munit_assert_int(cpu->PC, ==, 0x0002);
    }

    // JMP instruction should set the PC
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);
        TEST_SUCCESS();        
        munit_assert_int(cpu->PC, ==, TEST_MEMORY_ROM_HL);
    }

    {
        // Ensure there is no overflow
        SETUP_TEST_OVERFLOW_WORD(opcode);
    }

    // JMP instruction should fail, jumping to out of bounds
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_OOB_L, TEST_MEMORY_OOB_H);
        TEST_FAIL_GENERIC();
    }
}

// Unconditional jump
MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture)
{    
    // Setup CPU
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xC3);

    return MUNIT_OK;
}


/*
 * CALL - call function at address
 */

// Helper function for call functions
// We do not init the CPU in this
/*
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
}*/

void assert_call_function_cond(struct cpustate* cpu, uint8_t opcode, uint8_t true_flags, uint8_t false_flags)
{
    // Test overflow
    {
        SETUP_TEST_OVERFLOW_WORD(opcode);
    }

    // True condition
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->PSW = true_flags;

        TEST_SUCCESS();
        munit_assert_int(cpu->SP, ==, STACK_START - 2); // Stack pointer should decrease by two
        munit_assert_int(cpu->PC, ==, TEST_MEMORY_ROM_HL); // Current PC should point to new address
    }

    // False condition
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->PSW = false_flags;

        TEST_SUCCESS_WORD();
        munit_assert_int(cpu->SP, ==, STACK_START); // Stack pointer shouldn't decrease by two
        munit_assert_int(cpu->PC, !=, TEST_MEMORY_ROM_HL); // Current PC should point to new address
    }
}

// 
// Calls a function at an address
// This one is more in-depth than the conditional calls.
MunitResult
    test_cpuprocess_CD(const MunitParameter params[], void* fixture)
{
    struct cpustate realcpu;
    struct cpustate* cpu = &realcpu;

    // Ensure there is no overflow
    {
        SETUP_TEST_OVERFLOW_WORD(0xCD);
    }

    // Test functionality of CALL
    {
        SETUP_TEST_4(0x00, 0xCD,  TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        TEST_SUCCESS(); 
        TEST_SUCCESS(); // Process twice to get rid of NOP

        munit_assert_int(cpu->SP, ==, STACK_START - 2); // Stack pointer should decrease by two
        munit_assert_int(cpu->PC, ==, TEST_MEMORY_ROM_HL); // Current PC should point to new address
        printf("Memory: 0x%02X 0x%02X\n", program[cpu->SP + 1], program[cpu->SP + 2]);
        munit_assert_int(program[cpu->SP], ==, 0x00); // Stack should hold hi of return address
        munit_assert_int(program[cpu->SP + 1], ==, 0x01); // Stack + 1 should hold lo of return address
    }

    // Do not allow stack to underflow
    {
        SETUP_TEST_4(0x00, 0xCD,  TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->SP = 0x00;

        TEST_SUCCESS(); 
        TEST_FAIL(); // Process twice to get rid of NOP

        munit_assert_int(cpu->SP, ==, 0x00); // Stack pointer should stay the same
    }

    // Do not allow stack to write into ROM
    {
        SETUP_TEST_4(0x00, 0xCD,  TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->SP = TEST_MEMORY_ROM_HL;

        TEST_SUCCESS(); 
        TEST_FAIL(); // Process twice to get rid of NOP

        munit_assert_int(cpu->SP, ==, TEST_MEMORY_ROM_HL); // Stack pointer should stay the same
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

// CNZ
MunitResult
    test_cpuprocess_C4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xC4, get_psw_flag_z() , 0x00);
    return MUNIT_OK;
}

// CZ
MunitResult
    test_cpuprocess_CC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xCC, 0x00, get_psw_flag_z());
    return MUNIT_OK;
}

// CNC
MunitResult
    test_cpuprocess_D4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xD4, 0x00, get_psw_flag_c());
    return MUNIT_OK;
}

// CC
MunitResult
    test_cpuprocess_DC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xDC, get_psw_flag_c(), 0x00);
    return MUNIT_OK;
}

// CPO
MunitResult
    test_cpuprocess_E4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xE4, 0x00, get_psw_flag_p());
    return MUNIT_OK;
}

// CPE
MunitResult
    test_cpuprocess_EC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xEC, get_psw_flag_p(), 0x00);
    return MUNIT_OK;
}

// CP
MunitResult
    test_cpuprocess_F4(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xF4, 0x00, get_psw_flag_s());
    return MUNIT_OK;
}

// CM
MunitResult
    test_cpuprocess_FC(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xFC, get_psw_flag_s(), 0x00);
    return MUNIT_OK;
}


/*
 * MOV from memory to A functions
 */

// Helper function for transfer to A functions.
void assert_transfer_a_frommem(struct cpustate* cpu, uint8_t opcode, uint16_t* reg)
{
    // Ensure we load correctly
    {
        SETUP_TEST_1(opcode);

        program[TEST_MEMORY_RAM_HL] = TEST_MEMORY_BYTE;
        (*reg) = TEST_MEMORY_RAM_HL;

        TEST_SUCCESS_OPCODE();
        munit_assert_int(cpu->A, ==, TEST_MEMORY_BYTE);    // A should contain the memory loaded
    }

    // Ensure overflows fail
    {
        SETUP_TEST_1(opcode);

        program[TEST_MEMORY_RAM_HL] = TEST_MEMORY_BYTE;
        (*reg) = 0xFFFF;

        TEST_FAIL_GENERIC();

        munit_assert_int(cpu->A, !=, TEST_MEMORY_BYTE);    // A shouldn't contain the memory loaded
    }
}

// Load HL to A
MunitResult
    test_cpuprocess_7E(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_transfer_a_frommem(&cpu, 0x7E, &cpu.HL);
    return MUNIT_OK;
}

// Load BC to A
MunitResult
    test_cpuprocess_0A(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_transfer_a_frommem(&cpu, 0x0A, &cpu.BC);
    return MUNIT_OK;
}

// Load DE to A
MunitResult
    test_cpuprocess_1A(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_transfer_a_frommem(&cpu, 0x1A, &cpu.DE);
    return MUNIT_OK;
}


/*
 * MOV from 8-bit register to 16-bit memory
 */

// Helper function for transfer to memory from registers
void assert_mov_reg_to_mem(struct cpustate* cpu, uint8_t opcode, uint8_t* reg, uint8_t test_memory)
{
    // Ensure we load correctly
    {
        SETUP_TEST_1(opcode);

        (*reg) = test_memory;
        cpu->HL = TEST_MEMORY_RAM_HL;

        TEST_SUCCESS_OPCODE();

        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, test_memory);    // Memory should contain what the register had
    }

    // Ensure overflows fail
    {
        SETUP_TEST_1(opcode);

        (*reg) = test_memory;
        cpu->HL = TEST_MEMORY_OOB_HL;

        TEST_FAIL_GENERIC();

        munit_assert_int(program[TEST_MEMORY_RAM_HL], !=, test_memory);  
    }
}

// Move A to (HL)
MunitResult
    test_cpuprocess_77(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x77, &cpu.A, 0xAB);
    return MUNIT_OK;
}

// Move B to (HL)
MunitResult
    test_cpuprocess_70(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x70, &cpu.B, 0xAB);
    return MUNIT_OK;
}

// Move C to (HL)
MunitResult
    test_cpuprocess_71(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x71, &cpu.C, 0xAB);
    return MUNIT_OK;
}

// Move D to (HL)
MunitResult
    test_cpuprocess_72(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x72, &cpu.D, 0xAB);
    return MUNIT_OK;
}

// Move E to (HL)
MunitResult
    test_cpuprocess_73(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x73, &cpu.E, 0xAB);
    return MUNIT_OK;
}

// Move H to (HL)
MunitResult
    test_cpuprocess_74(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x74, &cpu.H, TEST_MEMORY_RAM_H);
    return MUNIT_OK;
}

// Move L to (HL)
MunitResult
    test_cpuprocess_75(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_mov_reg_to_mem(&cpu, 0x75, &cpu.L, TEST_MEMORY_RAM_L);
    return MUNIT_OK;
}

/*
 * INC byte
 */

// Helper function for increment functions
void assert_inc_byte(struct cpustate* cpu, uint8_t opcode, uint8_t* reg)
{
    // Ensure we increment normally
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 1);    
    }

    // Ensure increment wraps around
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0xFF;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0); 
    }
}

// a
MunitResult
    test_cpuprocess_3C(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x3C, &cpu.A);
    return MUNIT_OK;
}

// b
MunitResult
    test_cpuprocess_04(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x04, &cpu.B);
    return MUNIT_OK;
}

// c
MunitResult
    test_cpuprocess_0C(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x0C, &cpu.C);
    return MUNIT_OK;
}

// d
MunitResult
    test_cpuprocess_14(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x14, &cpu.D);
    return MUNIT_OK;
}

// e
MunitResult
    test_cpuprocess_1C(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x1C, &cpu.E);
    return MUNIT_OK;
}

// h
MunitResult
    test_cpuprocess_24(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x24, &cpu.H);
    return MUNIT_OK;
}

// l
MunitResult
    test_cpuprocess_2C(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_byte(&cpu, 0x2C, &cpu.L);
    return MUNIT_OK;
}

// (hl)
MunitResult
    test_cpuprocess_34(const MunitParameter params[], void* fixture)
{
    struct cpustate realcpu;
    struct cpustate* cpu = &realcpu;

    // Ensure we increment normally
    {
        SETUP_TEST_1(0x34);

        cpu->HL = TEST_MEMORY_RAM_HL;
        program[TEST_MEMORY_RAM_HL] = 0;

        TEST_SUCCESS_OPCODE();
        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, 1);    
    }

    // Ensure increment wraps around
    {
        SETUP_TEST_1(0x34);

        cpu->HL = TEST_MEMORY_RAM_HL;
        program[TEST_MEMORY_RAM_HL] = 0xFF;

        TEST_SUCCESS_OPCODE();
        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, 0); 
    }

    // Ensure OOB fails
    {
        SETUP_TEST_1(0x34);
        cpu->HL = TEST_MEMORY_OOB_HL;
        TEST_FAIL_GENERIC();
    }

    // Ensure no writing happens in ROM
    {
        SETUP_TEST_1(0x34);
        cpu->HL = TEST_MEMORY_ROM_HL;
        TEST_FAIL_GENERIC();
    }

    return MUNIT_OK;
}