#ifndef DISASM_TEST_H
#define DISASM_TEST_H

#include "../disasm.h"

// Known code should return success
MunitResult
    test_op_to_text(const MunitParameter params[], void* fixture);

// Should work for data 
MunitResult
    test_disassembly(const MunitParameter params[], void* fixture);

#endif
