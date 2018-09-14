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
 * LXI - words to registers 
 */
MunitResult
    test_cpuprocess_01(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_11(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_21(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_31(const MunitParameter params[], void* fixture);

/*
 * CALL - call function at address
 */
MunitResult
    test_cpuprocess_CD(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_C4(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_CC(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_D4(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_DC(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_E4(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_EC(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_F4(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_FC(const MunitParameter params[], void* fixture);

/* 
 * Jump commands 
 */
MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture);


/*
 * LOAD accumulator 
 */
MunitResult
    test_cpuprocess_7E(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_0A(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_1A(const MunitParameter params[], void* fixture);
    
#endif
