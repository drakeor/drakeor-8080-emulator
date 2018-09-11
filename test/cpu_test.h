#ifndef CPU_TEST_H_
#define CPU_TEST_H_

#include "../cpu.h"

// Known code should return success
MunitResult
    test_initcpu(const MunitParameter params[], void* fixture);

MunitResult
    test_initcpu_reset(const MunitParameter params[], void* fixture);

#endif
