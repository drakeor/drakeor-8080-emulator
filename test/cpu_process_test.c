#include "munit.h"
#include "../cpu.h"
#include "cpu_process_test.h"

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
 * CALL - call function at address
 */

// Helper function for call functions
void assert_call_function_cond(struct cpustate* cpu, uint8_t opcode, uint8_t true_flags, uint8_t false_flags)
{
    // Test overflow
    {
        SETUP_TEST_OVERFLOW_WORD(opcode);
    }

    // Test true condition
    {
        SETUP_TEST_5(0x00, opcode,  TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H, 0x00);

        cpu->PSW = true_flags;

        TEST_SUCCESS(); 
        TEST_SUCCESS(); // Process twice to get rid of NOP

        munit_assert_int(cpu->SP, ==, STACK_START - 2); // Stack pointer should decrease by two
        munit_assert_int(cpu->PC, ==, TEST_MEMORY_ROM_HL); // Current PC should point to new address
        printf("Memory: 0x%02X 0x%02X\n", program[cpu->SP + 1], program[cpu->SP + 2]);
        munit_assert_int(program[cpu->SP], ==, 0x00); // Stack should hold hi of return address
        munit_assert_int(program[cpu->SP + 1], ==, 0x01 + 0x03); // Stack + 1 should hold lo of return address + 3 (the next instruction after the original call)
    }

    // Test true condition and make sure stack doesn't underflow
    {
        SETUP_TEST_4(0x00, opcode,  TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->PSW = true_flags;
        cpu->SP = 0x00;

        TEST_SUCCESS(); 
        TEST_FAIL(); // Process twice to get rid of NOP

        munit_assert_int(cpu->SP, ==, 0x00); // Stack pointer should stay the same
    }

    // Do not allow stack to write into ROM
    {
        SETUP_TEST_4(0x00, opcode,  TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->PSW = true_flags;
        cpu->SP = TEST_MEMORY_ROM_HL;

        TEST_SUCCESS(); 
        TEST_FAIL(); // Process twice to get rid of NOP

        munit_assert_int(cpu->SP, ==, TEST_MEMORY_ROM_HL); // Stack pointer should stay the same
    }

    // Do not jump out of bounds
    {
        init_cpu(cpu);
        uint8_t program[4] = { 0x00, opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H };
        cpu->PSW = true_flags;
        process_cpu(cpu, program, 4); // Process twice to get rid of NOP
        int res = process_cpu(cpu, program, 4);
        munit_assert_int(res, ==, -1);   // Call should fail
    }

    // False condition (only if false flags are set)
    if(true_flags != false_flags) {
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
    struct cpustate cpu;
    assert_call_function_cond(&cpu, 0xCD, 0x00, 0x00);
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
 * MOV from memory to registers functions
 */
MunitResult
    test_cpuprocess_memory_to_register(const MunitParameter params[], void* fixture)
{
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Helper array to map bits to registers
    uint8_t *reg_mapping[8] = {
        &(r_cpu.B),
        &(r_cpu.C),
        &(r_cpu.D),
        &(r_cpu.E),
        &(r_cpu.H),
        &(r_cpu.L),
        NULL,
        &(r_cpu.A),      
    };

    // Register names
    char *reg_naming[8] = {
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "Invalid",
        "A"
    };

    // Run through each possible combination of register moves
   uint8_t dst = 0;
   for(dst = 0; dst < 8; dst++) {
        // Ignore the invalid move.
        if(dst != 6) {

            // Build the opcode
            uint8_t dst_byte = 0b00111000 & (dst << 3);
            uint8_t opcode = 0b01000110 | dst_byte;

            // Clear out registers and set the src one in question
            SETUP_TEST_1(opcode);
            program[TEST_MEMORY_RAM_HL] = TEST_MEMORY_BYTE;
            cpu->HL = TEST_MEMORY_RAM_HL;

            // Run opcode and make sure the source matches the destination opcode
            printf("Testing opcode %02X (MOV %s from (HL))\n", opcode, reg_naming[dst]);
            TEST_SUCCESS_OPCODE();
            munit_assert_int(TEST_MEMORY_BYTE, ==, (*reg_mapping[dst]));
        }
    }
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
    // Ensure increment wraps around
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0xFF;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0); 
        munit_assert_int(cpu->FLAGS.C, ==, 1);  
        munit_assert_int(cpu->FLAGS.Z, ==, 1);  

    }

    // Ensure we increment normally
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 1);    
        munit_assert_int(cpu->FLAGS.C, ==, 0);    
        munit_assert_int(cpu->FLAGS.Z, ==, 0);  

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

/*
 * DEC byte
 */

// Helper function for increment functions
void assert_dec_byte(struct cpustate* cpu, uint8_t opcode, uint8_t* reg)
{
    
    // Ensure increment wraps around
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0xFF); 
        munit_assert_int(cpu->FLAGS.C, ==, 1);  
        munit_assert_int(cpu->FLAGS.Z, ==, 0);  

    }
    
    // Ensure we decrement normally
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0x01;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0x00);    
        munit_assert_int(cpu->FLAGS.C, ==, 0);  
        munit_assert_int(cpu->FLAGS.Z, ==, 1);  
    }
}

// a
MunitResult
    test_cpuprocess_3D(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x3D, &cpu.A);
    return MUNIT_OK;
}

// b
MunitResult
    test_cpuprocess_05(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x05, &cpu.B);
    return MUNIT_OK;
}

// c
MunitResult
    test_cpuprocess_0D(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x0D, &cpu.C);
    return MUNIT_OK;
}

// d
MunitResult
    test_cpuprocess_15(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x15, &cpu.D);
    return MUNIT_OK;
}

// e
MunitResult
    test_cpuprocess_1D(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x1D, &cpu.E);
    return MUNIT_OK;
}

// h
MunitResult
    test_cpuprocess_25(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x25, &cpu.H);
    return MUNIT_OK;
}

// l
MunitResult
    test_cpuprocess_2D(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_byte(&cpu, 0x2D, &cpu.L);
    return MUNIT_OK;
}

// (hl)
MunitResult
    test_cpuprocess_35(const MunitParameter params[], void* fixture)
{
    struct cpustate realcpu;
    struct cpustate* cpu = &realcpu;

    // Ensure we increment normally
    {
        SETUP_TEST_1(0x35);

        cpu->HL = TEST_MEMORY_RAM_HL;
        program[TEST_MEMORY_RAM_HL] = 0xFF;

        TEST_SUCCESS_OPCODE();
        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, 0xFE);    
    }

    // Ensure increment wraps around
    {
        SETUP_TEST_1(0x35);

        cpu->HL = TEST_MEMORY_RAM_HL;
        program[TEST_MEMORY_RAM_HL] = 0x00;

        TEST_SUCCESS_OPCODE();
        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, 0xFF); 
    }

    // Ensure OOB fails
    {
        SETUP_TEST_1(0x35);
        cpu->HL = TEST_MEMORY_OOB_HL;
        TEST_FAIL_GENERIC();
    }

    // Ensure no writing happens in ROM
    {
        SETUP_TEST_1(0x35);
        cpu->HL = TEST_MEMORY_ROM_HL;
        TEST_FAIL_GENERIC();
    }

    return MUNIT_OK;
}

/*
 * INC word
 */

// Helper function for increment functions
void assert_inc_word(struct cpustate* cpu, uint8_t opcode, uint16_t* reg)
{
    // Ensure increment wraps around
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0xFFFF;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0x0000); 
        munit_assert_int(cpu->FLAGS.C, ==, 1); 
        munit_assert_int(cpu->FLAGS.Z, ==, 1);  
        
    }

    // Ensure we increment normally
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0x20FF;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0x2100);    
        munit_assert_int(cpu->FLAGS.C, ==, 0); 
        munit_assert_int(cpu->FLAGS.Z, ==, 0);  

    }
}

//bc
MunitResult
    test_cpuprocess_03(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_word(&cpu, 0x03, &cpu.BC);
    return MUNIT_OK;
}

//de
MunitResult
    test_cpuprocess_13(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_word(&cpu, 0x13, &cpu.DE);
    return MUNIT_OK;
}

// hl
MunitResult
    test_cpuprocess_23(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_word(&cpu, 0x23, &cpu.HL);
    return MUNIT_OK;
}

// sp
MunitResult
    test_cpuprocess_33(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_inc_word(&cpu, 0x33, &cpu.SP);
    return MUNIT_OK;
}

/*
 * DEC word
 */

// Helper function for decrement functions
void assert_dec_word(struct cpustate* cpu, uint8_t opcode, uint16_t* reg)
{
    // Ensure decrement wraps around
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0x0000;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0xFFFF); 
        munit_assert_int(cpu->FLAGS.C, ==, 1); 
        munit_assert_int(cpu->FLAGS.Z, ==, 0);  
    }
    // Ensure we decrement normally
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0x2000;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0x1FFF);  
        munit_assert_int(cpu->FLAGS.C, ==, 0);   
        munit_assert_int(cpu->FLAGS.Z, ==, 0);  
    }

    // Ensure we decrement normally and sets zero byte
    {
        SETUP_TEST_1(opcode);

        (*reg) = 0x0001;

        TEST_SUCCESS_OPCODE();
        munit_assert_int((*reg), ==, 0x0000);  
        munit_assert_int(cpu->FLAGS.C, ==, 0);   
        munit_assert_int(cpu->FLAGS.Z, ==, 1);  
    }
}

// bc
MunitResult
    test_cpuprocess_0B(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_word(&cpu, 0x0B, &cpu.BC);
    return MUNIT_OK;
}

// de
MunitResult
    test_cpuprocess_1B(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_word(&cpu, 0x1B, &cpu.DE);
    return MUNIT_OK;
}

// hl
MunitResult
    test_cpuprocess_2B(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_word(&cpu, 0x2B, &cpu.HL);
    return MUNIT_OK;
}

// sp
MunitResult
    test_cpuprocess_3B(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_dec_word(&cpu, 0x3B, &cpu.SP);
    return MUNIT_OK;
}


/*
 * JMPS to boundaries
 */

// JMP to word
void assert_jump_opcode(struct cpustate* cpu, uint8_t opcode, uint8_t true_flags, uint8_t false_flags)
{

    // Ensure there is no overflow
    {
        SETUP_TEST_OVERFLOW_WORD(opcode);
    }

    // JMP instruction should set the PC to 0x0002
    {
        SETUP_TEST_3(opcode, 0x02, 0x00);
        
        cpu->PSW = true_flags;

        TEST_SUCCESS();
        munit_assert_int(cpu->PC, ==, 0x0002);
    }

    // JMP instruction should set the PC
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->PSW = true_flags;

        TEST_SUCCESS();        
        munit_assert_int(cpu->PC, ==, TEST_MEMORY_ROM_HL);
    }

    // Only test false if true_flags and false_flags aren't equal.
    if(true_flags != false_flags) {
        SETUP_TEST_3(opcode, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);

        cpu->PSW = false_flags;

        TEST_SUCCESS_WORD();
    }

    // JMP instruction should fail, jumping to out of bounds
    {
        SETUP_TEST_3(opcode, TEST_MEMORY_OOB_L, TEST_MEMORY_OOB_H);
        TEST_FAIL_GENERIC();
    }
}

// Helper function for call functions

// Unconditional jump
MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture)
{    
    // Setup CPU
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xC3, 0x00, 0x00);

    return MUNIT_OK;
}

// if z == 0
MunitResult
    test_cpuprocess_C2(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xC2, 0x00, get_psw_flag_z());

    return MUNIT_OK;
}

// if z == 1
MunitResult
    test_cpuprocess_CA(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xCA, get_psw_flag_z(), 0x00);

    return MUNIT_OK;
}

// if c == 0
MunitResult
    test_cpuprocess_D2(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xD2, 0x00, get_psw_flag_c());

    return MUNIT_OK;
}

// if c == 1
MunitResult
    test_cpuprocess_DA(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xDA, get_psw_flag_c(), 0x00);

    return MUNIT_OK;
}

// if p == 0
MunitResult
    test_cpuprocess_E2(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xE2, 0x00, get_psw_flag_p());

    return MUNIT_OK;
}

// if p = 1
MunitResult
    test_cpuprocess_EA(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xEA, get_psw_flag_p(), 0x00);

    return MUNIT_OK;
}

// if s == 0
MunitResult
    test_cpuprocess_F2(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xF2, 0x00, get_psw_flag_s());

    return MUNIT_OK;
}

// if s == 1
MunitResult
    test_cpuprocess_FA(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;
    assert_jump_opcode(&cpu, 0xFA, get_psw_flag_s(), 0x00);

    return MUNIT_OK;
}


/*
 * Helper function for RET function
 */
void assert_ret_byte(struct cpustate* cpu, uint8_t opcode)
{
     // First test, make sure it's working correctly by manually adding data to the stack and testing it
    {
        SETUP_TEST_1(0xC9);

        cpu->SP = cpu->SP - 2;
        program[cpu->SP] = TEST_MEMORY_RAM_L;
        program[cpu->SP+1] = TEST_MEMORY_RAM_H;

        TEST_SUCCESS();

        munit_assert_int(cpu->SP, ==, STACK_START);  
        munit_assert_int(cpu->PC, ==, TEST_MEMORY_RAM_LH);  

    }

    // Second test, make sure we don't underflow and nothing is changed if it fails
    {
        SETUP_TEST_1(0xC9);

        TEST_FAIL();

        munit_assert_int(cpu->SP, ==, STACK_START);  
        munit_assert_int(cpu->PC, ==, 0x00);  
    }

}

// Generic return function
MunitResult
    test_cpuprocess_C9(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    assert_ret_byte(&cpu, 0xC9);
   
	return MUNIT_OK;
}

/*
 * Helper function for MVI command
 */
void assert_mvi_addr_byte(struct cpustate* cpu, uint8_t opcode)
{
    // Test true condition and make sure it was loaded to memory
    {
        SETUP_TEST_2(opcode,  TEST_MEMORY_BYTE);

        cpu->HL = TEST_MEMORY_RAM_HL;

        TEST_SUCCESS_BYTE(); 

        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, TEST_MEMORY_BYTE);
    }

    // Test false condition where we try to load into ROM (not allowed)
    {
        SETUP_TEST_2(opcode,  TEST_MEMORY_BYTE);

        cpu->HL = TEST_MEMORY_ROM_HL;

        TEST_FAIL_GENERIC();

        munit_assert_int(program[TEST_MEMORY_RAM_HL], !=, TEST_MEMORY_BYTE);
    }

    // Test false condition where we try to load out of bounds (not allowed)
    {
        SETUP_TEST_2(opcode,  TEST_MEMORY_BYTE);

        cpu->HL = TEST_MEMORY_OOB_HL;

        TEST_FAIL_GENERIC();

        munit_assert_int(program[TEST_MEMORY_RAM_HL], !=, TEST_MEMORY_BYTE);
    }
}

// Loads immediate to (HL)
MunitResult
    test_cpuprocess_36(const MunitParameter params[], void* fixture)
{
    struct cpustate cpu;

    assert_mvi_addr_byte(&cpu, 0x36);
   
	return MUNIT_OK;
}


// Mov register to register tests
MunitResult
    test_cpuprocess_mov_reg_to_reg(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Helper array to map bits to registers
    uint8_t *reg_mapping[8] = {
        &(r_cpu.B),
        &(r_cpu.C),
        &(r_cpu.D),
        &(r_cpu.E),
        &(r_cpu.H),
        &(r_cpu.L),
        NULL,
        &(r_cpu.A),      
    };

    // Register names
    char *reg_naming[8] = {
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "Invalid",
        "A"
    };

   // Run through each possible combination of register moves
   uint8_t dst = 0;
   uint8_t src = 0;
   for(dst = 0; dst < 8; dst++) {
       for(src = 0; src < 8; src++) {
           
            // Ignore the memory moves. These are covered in other tests already
            if(dst != 6 && src != 6) {

                // Build the opcode
                uint8_t src_byte = 0b00000111 & src;
                uint8_t dst_byte = 0b00111000 & (dst << 3);
                uint8_t opcode = 0b01000000 | dst_byte | src_byte;

                // Clear out registers and set the src one in question
                SETUP_TEST_1(opcode);
                (*reg_mapping[src]) = 0x1;

                // Run opcode and make sure the source matches the destination opcode
                printf("Testing opcode %02X (MOV %s from %s)\n", opcode, reg_naming[dst], reg_naming[src]);
                TEST_SUCCESS_OPCODE();
                munit_assert_int((*reg_mapping[src]), ==, (*reg_mapping[dst]));
            }
       }
   }
	return MUNIT_OK;
}

// CPI tests
MunitResult
    test_cpuprocess_FE(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // 74 - 64 = 10 (No carry / No zero)
    {
        SETUP_TEST_2(0xFE, 0x40);
        cpu->A = 0x4A;
        TEST_SUCCESS_BYTE();

        munit_assert_int(cpu->FLAGS.C, ==, 0);
        munit_assert_int(cpu->FLAGS.Z, ==, 0);
    }

    // 64 - 64 = 0 (No carry / Zero)
    {
        SETUP_TEST_2(0xFE, 0x40);
        cpu->A = 0x40;
        TEST_SUCCESS_BYTE();

        munit_assert_int(cpu->FLAGS.C, ==, 0);
        munit_assert_int(cpu->FLAGS.Z, ==, 1);
    }

    // 64 - 74 = -10 (Carry / No zero)
    {
        SETUP_TEST_2(0xFE, 0x4A);
        cpu->A = 0x40;
        TEST_SUCCESS_BYTE();

        munit_assert_int(cpu->FLAGS.C, ==, 1);
        munit_assert_int(cpu->FLAGS.Z, ==, 0);
    }
}

/*
 * PUSH commands
 */
MunitResult
    test_cpuprocess_push(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // This is DIFFERENT that SP is actually PSW as we can't push/pop SP
    // We're gonna leave PSW out for now and get back to it later since the
    // logic is slightly different
    uint16_t *regpair_pushpop_mapping[3] = {
        &(r_cpu.BC), 
        &(r_cpu.DE),
        &(r_cpu.HL)  
    };
    char *regpair_pushpop_naming[3] = {
        "BC",
        "DE",
        "HL"
    };

    uint8_t i = 0;
    for(i = 0; i < 3; i++) {
        // Build the opcode
        uint8_t dst_byte = 0b00110000 & (i << 4);
        uint8_t opcode = 0b11000101 | dst_byte;

         // Make sure opcode works as expected
        {
            SETUP_TEST_1(opcode);
            (*regpair_pushpop_mapping[i]) = TEST_MEMORY_ROM_LH;

            printf("Testing Opcode %02X (REG %s) works\n", opcode, regpair_pushpop_naming[i]);
            TEST_SUCCESS_OPCODE();

            munit_assert_int(cpu->SP, ==, STACK_START - 2);
            munit_assert_int(program[STACK_START-1], ==, TEST_MEMORY_ROM_L);
            munit_assert_int(program[STACK_START-2], ==, TEST_MEMORY_ROM_H);
        }

        // Make sure opcode doesn't underflow
        {
            SETUP_TEST_1(opcode);
            cpu->SP = TEST_MEMORY_ROM_HL;
            (*regpair_pushpop_mapping[i]) = TEST_MEMORY_ROM_LH;

            printf("Testing Opcode %02X (REG %s) underflow\n", opcode, regpair_pushpop_naming[i]);
            TEST_FAIL_GENERIC();

            munit_assert_int(cpu->SP, ==, TEST_MEMORY_ROM_HL);
        }
    }
    
    return MUNIT_OK;
}

MunitResult
    test_cpuprocess_push_psw(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Test for success
    {
        SETUP_TEST_1(0xF5);
        cpu->PSW = TEST_MEMORY_BYTE;
        cpu->A = TEST_MEMORY_BYTE & 0x0F;

        TEST_SUCCESS_OPCODE();
        munit_assert_int(cpu->SP, ==, STACK_START - 2);
        munit_assert_int(program[STACK_START-1], ==, TEST_MEMORY_BYTE & 0x0F);
        munit_assert_int(program[STACK_START-2], ==, TEST_MEMORY_BYTE);
    }

    // Test for underflow
    {
        SETUP_TEST_1(0xF5);
        cpu->SP = TEST_MEMORY_ROM_HL;
        TEST_FAIL_GENERIC();
        munit_assert_int(cpu->SP, ==, TEST_MEMORY_ROM_HL);
    }
}

MunitResult
    test_cpuprocess_dad(const MunitParameter params[], void* fixture)
{
    
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Helper array to map bits to registers
    uint16_t *regpair_mapping[4] = {
        &(r_cpu.BC),
        &(r_cpu.DE),
        &(r_cpu.HL),
        &(r_cpu.SP),    
    };

    // Register names
    char *regpair_naming[4] = {
        "BC",
        "DE",
        "HL",
        "SP"
    };

    uint8_t i = 0;
    for(i = 0; i < 4; i++) {

         // Build the opcode
        uint8_t dst_byte = 0b00110000 & (i << 4);
        uint8_t opcode = 0b00001001 | dst_byte;

        // 42F0‬ + 42F0‬ = ‭85E0 ‬(No carry)
        {
            // Clear out registers and set the src one in question
            SETUP_TEST_1(opcode);
            cpu->HL = 0x42F0;
            (*regpair_mapping[i]) = 0x42F0;

            // Run opcode and make sure the source matches the destination opcode
            printf("Testing Opcode %02X (REG %s) No Carry\n", opcode, regpair_naming[i]);
            TEST_SUCCESS_OPCODE();
            munit_assert_int(cpu->HL, ==, 0x85E0);
            munit_assert_int(cpu->FLAGS.C, ==, 0);
        }

        // D51A + D51A = AA34‬ (Carry)
        {
            // Clear out registers and set the src one in question
            SETUP_TEST_1(opcode);
            cpu->HL = 0xD51A;
            (*regpair_mapping[i]) = 0xD51A;

            // Run opcode and make sure the source matches the destination opcode
            printf("Testing Opcode %02X (REG %s) Carry\n", opcode, regpair_naming[i]);
            TEST_SUCCESS_OPCODE();
            munit_assert_int(cpu->HL, ==, 0xAA34);
            munit_assert_int(cpu->FLAGS.C, ==, 1);
        }
    }
}

MunitResult
    test_cpuprocess_EB(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    SETUP_TEST_1(0xEB);
    cpu->HL = 0xD51A;
    cpu->DE = 0xAA34;

    TEST_SUCCESS_OPCODE();
    munit_assert_int(cpu->HL, ==, 0xAA34);
    munit_assert_int(cpu->DE, ==, 0xD51A);
}

MunitResult
    test_cpuprocess_pop(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // This is DIFFERENT that SP is actually PSW as we can't push/pop SP
    // We're gonna leave PSW out for now and get back to it later since the
    // logic is slightly different
    uint16_t *regpair_pushpop_mapping[3] = {
        &(r_cpu.BC),
        &(r_cpu.DE),
        &(r_cpu.HL)  
    };
    char *regpair_pushpop_naming[3] = {
        "BC",
        "DE",
        "HL"
    };

    uint8_t i = 0;
    for(i = 0; i < 3; i++) {

         // Build the opcode
        uint8_t dst_byte = 0b00110000 & (i << 4);
        uint8_t opcode = 0b11000001 | dst_byte;

        printf("Testing Opcode %02X (REG %s) Carry\n", opcode, regpair_pushpop_naming[i]);

        // Ensure pops correctly
        {
            // Clear out registers and set the src one in question
            SETUP_TEST_1(opcode);

            cpu->SP = cpu->SP - 2;
            program[cpu->SP] = TEST_MEMORY_RAM_L;
            program[cpu->SP+1] = TEST_MEMORY_RAM_H;

            TEST_SUCCESS_OPCODE();

            munit_assert_int(cpu->SP, ==, STACK_START);  
            munit_assert_int((*regpair_pushpop_mapping[i]), ==, TEST_MEMORY_RAM_HL);  
        }

        // Ensure stack doesn't underflow
        {
            SETUP_TEST_1(opcode);
            TEST_FAIL_GENERIC();
            munit_assert_int(cpu->SP, ==, STACK_START);  
        }
    }

    return MUNIT_OK;
}

// Pop PSW
MunitResult
    test_cpuprocess_pop_psw(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Ensure pops correctly
    {
        // Clear out registers and set the src one in question
        SETUP_TEST_1(0xF1);

        cpu->SP = cpu->SP - 2;
        program[cpu->SP] = TEST_MEMORY_RAM_L;
        program[cpu->SP+1] = TEST_MEMORY_RAM_H;

        TEST_SUCCESS_OPCODE();

        munit_assert_int(cpu->SP, ==, STACK_START);  
        munit_assert_int(cpu->PSW, ==, TEST_MEMORY_RAM_L);  
        munit_assert_int(cpu->A, ==, TEST_MEMORY_RAM_H);  
    }

    // Ensure stack doesn't underflow
    {
        SETUP_TEST_1(0xF1);
        TEST_FAIL_GENERIC();
        munit_assert_int(cpu->SP, ==, STACK_START);  
    }
}

// OUT instruction emulation
MunitResult
    test_cpuprocess_D3(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Just make sure it runs for now i guess
    SETUP_TEST_2(0xD3, 0b10100000);
    TEST_SUCCESS_BYTE();
    return MUNIT_OK;
}

/*
 * Register Rotation Instructions
 */
 MunitResult
    test_cpuprocess_register_rotations(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // RLC Command
    {
        printf("\nTesting Opcode 0x07\n");
        SETUP_TEST_1(0x07);
        // A << 1, Bit 0 of A is previous bit 7 of A. Bit 7 is assigned to carry flag.
        cpu->A = TEST_MEMORY_BYTE;
        uint8_t prev_bit_7 = ((TEST_MEMORY_BYTE >> 7) & 0x1);
        uint8_t rotated_test_byte = (((uint8_t)TEST_MEMORY_BYTE) << 1) | prev_bit_7; 
        TEST_SUCCESS_OPCODE();

        munit_assert_int(cpu->A, ==, rotated_test_byte);  
        munit_assert_int(cpu->FLAGS.C, ==, prev_bit_7);
    }

    // RRC Command
    {
        printf("Testing Opcode 0x0F\n");
        SETUP_TEST_1(0x0F);
        // A << 1, Bit 7 of A is previous bit 0 of A. Bit 0 is assigned to carry flag.
        cpu->A = TEST_MEMORY_BYTE;
        uint8_t prev_bit_0 = TEST_MEMORY_BYTE & 0x1;
        uint8_t rotated_test_byte = (((uint8_t)TEST_MEMORY_BYTE) >> 1) | (prev_bit_0 << 7); 
        TEST_SUCCESS_OPCODE();

        munit_assert_int(cpu->A, ==, rotated_test_byte);  
        munit_assert_int(cpu->FLAGS.C, ==, prev_bit_0);
    }

    return MUNIT_OK;
}

/*
 * Bitwise Operations with accumulator and immediate
 */
#define BITWISE_ACC_NUM_OPS 4
MunitResult
    test_cpuprocess_bitwise_accumulator(const MunitParameter params[], void* fixture)
{
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    uint8_t op_results[BITWISE_ACC_NUM_OPS] = {
        (TEST_MEMORY_BYTE & TEST_MEMORY_BYTE_2),
        (TEST_MEMORY_BYTE ^ TEST_MEMORY_BYTE_2),
        (TEST_MEMORY_BYTE | TEST_MEMORY_BYTE_2),
        (uint8_t)((uint8_t)TEST_MEMORY_BYTE + (uint8_t)TEST_MEMORY_BYTE_2)
    };

    uint8_t op_code[BITWISE_ACC_NUM_OPS] = {
        0xE6,
        0xEE,
        0xF6,
        0xC6
    };

    uint8_t i ;
    for(i = 0; i < BITWISE_ACC_NUM_OPS; i++) {
        printf("\nTesting Opcode %02X\n", op_code[i]);
        SETUP_TEST_2(op_code[i], TEST_MEMORY_BYTE);
        cpu->A = TEST_MEMORY_BYTE_2;
        cpu->FLAGS.C = 1;
        TEST_SUCCESS_BYTE();

        // A should be A & data. Carry should be reset, Zero should be not reset, Sign bit is also set
        munit_assert_int(cpu->A, ==, op_results[i]);  
        munit_assert_int(cpu->FLAGS.C, ==, 0);
        munit_assert_int(cpu->FLAGS.Z, ==, (op_results[i] == 0));
        munit_assert_int(cpu->FLAGS.S, ==, (op_results[i] >> 7) & 0x1);
    }

    // Make sure doesn't overflow
    {
        SETUP_TEST_OVERFLOW_BYTE(0xE6);
        munit_assert_int((*cpu).PC, ==, 0);
    }

    return MUNIT_OK;
}

/*
 * Store/Load address to accumulator
 */

 // Load
MunitResult
    test_cpuprocess_3A(const MunitParameter params[], void* fixture)
{
    struct cpustate realcpu;
    struct cpustate* cpu = &realcpu;

    // Ensure we load normally
    {
        SETUP_TEST_3(0x3A, TEST_MEMORY_RAM_L, TEST_MEMORY_RAM_H);
        program[TEST_MEMORY_RAM_HL] = TEST_MEMORY_BYTE;
        TEST_SUCCESS_WORD();
        munit_assert_int(cpu->A, ==, TEST_MEMORY_BYTE);    
    }

    // Ensure overflows fail
    {
        SETUP_TEST_3(0x3A, TEST_MEMORY_OOB_L, TEST_MEMORY_OOB_H);
        TEST_FAIL_GENERIC();
    }

    return MUNIT_OK;
}

// Store
MunitResult
    test_cpuprocess_32(const MunitParameter params[], void* fixture)
{
    struct cpustate realcpu;
    struct cpustate* cpu = &realcpu;

    // Ensure we store normally
    {
        SETUP_TEST_3(0x32, TEST_MEMORY_RAM_L, TEST_MEMORY_RAM_H);
        cpu->A = TEST_MEMORY_BYTE;
        TEST_SUCCESS_WORD();
        munit_assert_int(program[TEST_MEMORY_RAM_HL], ==, TEST_MEMORY_BYTE);    
    }

    // Ensure overflows fail
    {
        SETUP_TEST_3(0x32, TEST_MEMORY_OOB_L, TEST_MEMORY_OOB_H);
        cpu->A = TEST_MEMORY_BYTE;
        TEST_FAIL_GENERIC();
    }

    // Ensure we don't write into ROM
    {
        SETUP_TEST_3(0x32, TEST_MEMORY_ROM_L, TEST_MEMORY_ROM_H);
        cpu->A = TEST_MEMORY_BYTE;
        TEST_FAIL_GENERIC();
    }
}

MunitResult
    test_cpuprocess_bitwise_accum_reg(const MunitParameter params[], void* fixture)
{
    // Doing this allows our macros to work
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;

    // Helper array to map bits to registers
    // Position 6 in array is mem access. We need to handle that specially.
    uint8_t *reg_mapping[8] = {
        &(r_cpu.B),
        &(r_cpu.C),
        &(r_cpu.D),
        &(r_cpu.E),
        &(r_cpu.H),
        &(r_cpu.L),
        NULL,
        &(r_cpu.A),      
    };

    // Register names
    char *reg_naming[8] = {
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "Memory[HL]",
        "A"
    };

    uint8_t i = 0;
    uint8_t j = 0;
    // i = operator to test.
    
    for(i = 0; i < 3; i++) {

        for(j = 0; j < 8; j++) {

            // Skip over memory operations for now
            if(j == 6)
                continue;

            // Construct the opcodes to test
            uint8_t opcode = 0;
            if(i == 0) {
                opcode = 0b10100000 | j;
            }
            if(i == 1) {
                opcode = 0b10101000 | j;
            }
            if(i == 2) {
                opcode = 0b10110000 | j;
            }

            printf("\nTesting Opcode %02X", opcode);

            SETUP_TEST_1(opcode);
            cpu->A = TEST_MEMORY_BYTE;
            (*reg_mapping[j]) = TEST_MEMORY_BYTE_2;
            cpu->FLAGS.C = 1;

            uint8_t op_result = 0;
            if(i == 0) {
                op_result = cpu->A & (*reg_mapping[j]);
            }
            if(i == 1) {
                op_result =  cpu->A ^ (*reg_mapping[j]);
            }
            if(i == 2) {
                op_result =  cpu->A | (*reg_mapping[j]);
            }

            TEST_SUCCESS_OPCODE();

            // A should be A & data. Carry should be reset, Zero should be not reset, Sign bit is also set
            munit_assert_int(cpu->A, ==, op_result);  
            munit_assert_int(cpu->FLAGS.C, ==, 0);
            munit_assert_int(cpu->FLAGS.Z, ==, (op_result == 0));
            munit_assert_int(cpu->FLAGS.S, ==, (op_result >> 7) & 0x1);
        }
    }

    return MUNIT_OK;

}

MunitResult
    test_cpuprocess_interrupts(const MunitParameter params[], void* fixture)
{
    struct cpustate r_cpu;
    struct cpustate* cpu = &r_cpu;
    
    // Disable Interrupts
    {
        SETUP_TEST_1(0xF3);
        cpu->INTE = 1;
        TEST_SUCCESS_OPCODE();
        munit_assert_int(cpu->INTE, ==, 0x0);
    }
    // Enable interrupts
    {
        SETUP_TEST_1(0xFB);
        TEST_SUCCESS_OPCODE();
        munit_assert_int(cpu->INTE, ==, 0x1);
    }
    return MUNIT_OK;
}