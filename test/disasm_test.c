#include "munit.h"
#include "disasm_test.h"

char blank_data[2] = { 0x00, 0x00 };

// Known code should return success
MunitResult
    test_op_to_text(const MunitParameter params[], void* fixture) {
        int pc = 0;
        munit_assert_int(op_to_text(blank_data, &pc), ==, 0);
        return MUNIT_OK;
}

// Should work for data 
MunitResult
    test_disassembly(const MunitParameter params[], void* fixture) {
        munit_assert_int(disassemble(blank_data, 2), ==, 0);
        return MUNIT_OK;
}
