#include "munit.h"
#include "disasm_test.h"
#include "../cpu.h"

#define PRGM_DATA_SIZE 2
char program_data[PRGM_DATA_SIZE] = { 0x00, 0x00 };

char invalid_op_program[1] = { 0xFF };

// Known code should return success
// Unkwown code should return -1
// Buffer overflows or errors should return -2
MunitResult
    test_op_to_text(const MunitParameter params[], void* fixture) {
        int pc = 0;
        munit_assert_int(op_to_text(invalid_op_program, 1, &pc), ==, -1);
        pc = 0;
        munit_assert_int(op_to_text(program_data, PRGM_DATA_SIZE, &pc), ==, 0);
        pc = 0xFFFFAA;
        munit_assert_int(op_to_text(program_data, PRGM_DATA_SIZE, &pc), ==, -2);
        return MUNIT_OK;
}

// Should work for data or no data 
MunitResult
    test_disassembly(const MunitParameter params[], void* fixture) {
        munit_assert_int(disassemble((unsigned char *)NULL, 0), ==, 0);
        munit_assert_int(disassemble(program_data, PRGM_DATA_SIZE), ==, 0);
        return MUNIT_OK;
}

// Tests all opcodes. Makes sure nothing crashes the function
MunitResult
    test_op_to_text_all(const MunitParameter params[], void* fixture)
{
    for(int i=0;i<0xFF;i++) {
        int pc = 0;
        char program_opcode[1] = { (char)i };
        op_to_text(program_opcode, 1, &pc);
    }

    return MUNIT_OK;
}
