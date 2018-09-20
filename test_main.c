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

    /*
     * Disassembly and op_to_text operations
     */
    {"/disasm/op_to_text", test_op_to_text, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/disasm/op_to_text_all", test_op_to_text_all, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/disasm/disassembly", test_disassembly, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    /*
     * CPU Logic
     */
    {"/cpu/cpuinit", test_initcpu, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu/cpuinit_reset", test_initcpu_reset, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu/cpuinit_registers", test_initcpu_registers, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu/dump_registers", test_dump_registers, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    /* 
    * LD bytes to registers 
    */
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

    /* 
    * LXI - words to registers 
    */
   {"/cpu_process/01",test_cpuprocess_01, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/11",test_cpuprocess_11, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/21",test_cpuprocess_21, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/31",test_cpuprocess_31, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    // Jump instruction
    {"/cpu_process/C3",test_cpuprocess_C3, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    /*
     * CALL to address
     */
    {"/cpu_process/CD",test_cpuprocess_CD, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/C4",test_cpuprocess_C4, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/CC",test_cpuprocess_CC, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/D4",test_cpuprocess_D4, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/DC",test_cpuprocess_DC, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/E4",test_cpuprocess_E4, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/EC",test_cpuprocess_EC, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/F4",test_cpuprocess_F4, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/FC",test_cpuprocess_FC, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    /*
     * MOV from memory to A
     */
    {"/cpu_process/7E",test_cpuprocess_7E, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/0A",test_cpuprocess_0A, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/1A",test_cpuprocess_1A, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    /*
     * MOV from register to memory
     */
    {"/cpu_process/77",test_cpuprocess_77, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/70",test_cpuprocess_70, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/71",test_cpuprocess_71, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/72",test_cpuprocess_72, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/73",test_cpuprocess_73, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/74",test_cpuprocess_74, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/75",test_cpuprocess_75, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    /*
     * INC BYTE
     */
    {"/cpu_process/3C",test_cpuprocess_3C, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/04",test_cpuprocess_04, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/0C",test_cpuprocess_0C, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/14",test_cpuprocess_14, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/1C",test_cpuprocess_1C, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/24",test_cpuprocess_24, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/2C",test_cpuprocess_2C, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    {"/cpu_process/34",test_cpuprocess_34, 
        NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },

    // Required
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

