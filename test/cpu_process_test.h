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
 * MOV from memory to register
 */
MunitResult
    test_cpuprocess_7E(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_0A(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_1A(const MunitParameter params[], void* fixture);

/*
 * MOV from register to memory
 */
MunitResult
    test_cpuprocess_77(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_70(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_71(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_72(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_73(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_74(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_75(const MunitParameter params[], void* fixture);

/*
 * INC byte
 */
MunitResult
    test_cpuprocess_3C(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_04(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_0C(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_14(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_1C(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_24(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_2C(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_34(const MunitParameter params[], void* fixture);

/*
 * DEC byte
 */
MunitResult
    test_cpuprocess_3D(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_05(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_0D(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_15(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_1D(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_25(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_2D(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_35(const MunitParameter params[], void* fixture);

/*
 * INC register pair
 */
MunitResult
    test_cpuprocess_03(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_13(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_23(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_33(const MunitParameter params[], void* fixture);

/*
 * DEC register pair
 */
MunitResult
    test_cpuprocess_0B(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_1B(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_2B(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_3B(const MunitParameter params[], void* fixture);

/* 
 * JMP commands 
 */
MunitResult
    test_cpuprocess_C3(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_C2(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_CA(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_D2(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_DA(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_E2(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_EA(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_F2(const MunitParameter params[], void* fixture);
MunitResult
    test_cpuprocess_FA(const MunitParameter params[], void* fixture);

#endif
