#ifndef CPU_PROCESS_TEST_H_
#define CPU_PROCESS_TEST_H_

#include "../cpu.h"

/* 
 * LD bytes to registers 
 */
MunitResult
    test_cpuprocess_3E(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_06(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_0E(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_16(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_1E(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_26(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_2E(const MunitParameter params[], void* fixture);

/* 
 * LD words to registers 
 */
MunitResult
    test_cpuprocess_31(const MunitParameter params[], void* fixture);

/* 
 * Jump commands 
 */
MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture);
#endif
