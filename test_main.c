#include <stdlib.h>
#include <stdio.h>
#include "disasm.h"
#include "cpu.h"
#include "munit.h"

// Uses munit
// https://nemequ.github.io/munit/#getting-started

char blank_data[2] = { 0x00, 0x00 };

// Known code should return success
static MunitResult
    test_op_to_text(const MunitParameter params[], void* fixture) {
        int pc = 0;
        munit_assert_int(op_to_text(blank_data, &pc), ==, 0);
        return MUNIT_OK;
}

// Should work for data 
static MunitResult
    test_disassembly(const MunitParameter params[], void* fixture) {
        munit_assert_int(disassemble(blank_data, 2), ==, 0);
        return MUNIT_OK;
}


MunitTest tests[] = {
  {
    "/disasm/op_to_text", /* name */
    test_op_to_text, /* test */
    NULL, /* setup */
    NULL, /* tear_down */
    MUNIT_TEST_OPTION_NONE, /* options */
    NULL /* parameters */
  },
  {
    "/disasm/disassembly", /* name */
    test_disassembly, /* test */
    NULL, /* setup */
    NULL, /* tear_down */
    MUNIT_TEST_OPTION_NONE, /* options */
    NULL /* parameters */
  },

  /* Mark the end of the array with an entry where the test
   * function is NULL */
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
  "/unit-tests", /* name */
  tests, /* tests */
  NULL, /* suites */
  1, /* iterations */
  MUNIT_SUITE_OPTION_NONE /* options */
};

int main (int argc, const char* argv[]) {
  return munit_suite_main(&suite, NULL, argc, argv);
}

