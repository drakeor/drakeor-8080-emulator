#include <stdlib.h>
#include <stdio.h>

// Uses munit
// https://nemequ.github.io/munit/#getting-started
#include "test/munit.h"
#include "test/disasm_test.h"
#include "test/cpu_test.h"
#include "test/cpu_process_test.h"

// List of tests
MunitTest tests[] = {
    {"/disasm/op_to_text", test_op_to_text, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/disasm/op_to_text_all", test_op_to_text_all, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/disasm/disassembly", test_disassembly, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    {"/cpu/cpuinit", test_initcpu, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu/cpuinit_reset", test_initcpu_reset, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu/cpuinit_registers", test_initcpu_registers, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu/dump_registers", test_dump_registers, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    {"/cpu_process/3E",test_cpuprocess_3E, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/06",test_cpuprocess_06, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/0E",test_cpuprocess_0E, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/16",test_cpuprocess_16, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/1E",test_cpuprocess_1E, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/26",test_cpuprocess_26, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/2E",test_cpuprocess_2E, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    {"/cpu_process/C3",test_cpuprocess_C3, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/31",test_cpuprocess_31, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

// Test Suite
static const MunitSuite suite = {
  "/unit-tests", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

// Program entry point
int main (int argc, const char* argv[]) {
  return munit_suite_main(&suite, NULL, argc, (char* const*)argv);
}

