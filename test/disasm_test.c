#include "munit.h"
#include "disasm_test.h"

#define PRGM_DATA_SIZE 2
char program_data[PRGM_DATA_SIZE] = { 0x00, 0x78 };

char invalid_op_program[PRGM_DATA_SIZE] = { 0xFF };

// Known code should return success
// Unkwown code should return failure
MunitResult
    test_op_to_text(const MunitParameter params[], void* fixture) {
        int pc = 0;
        munit_assert_int(op_to_text(invalid_op_program, &pc), !=, 0);
        pc = 0;
        munit_assert_int(op_to_text(program_data, &pc), ==, 0);
        return MUNIT_OK;
}

// Should work for data or no data 
MunitResult
    test_disassembly(const MunitParameter params[], void* fixture) {
        munit_assert_int(disassemble((unsigned char *)NULL, 0), ==, 0);
        munit_assert_int(disassemble(program_data, PRGM_DATA_SIZE), ==, 0);
        return MUNIT_OK;
}
