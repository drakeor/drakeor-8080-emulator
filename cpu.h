#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Holds the current state of the CPU
struct cpustate {
    
    // A is the primary 8-bit accumulator
    uint8_t A;
    
    // 3 16-bit registers (BC, DE, HL)
    // that can function as 6 8-bit registers (B, C, D, E, H, L)
    union {
        struct {
            uint8_t B;
            uint8_t C;
        } s;
        uint16_t b;
    } BC;
    union {
        struct {
            uint8_t D;
            uint8_t E;
        } s;
        uint16_t b;
    } DE;
    union {
        struct {
            uint8_t H;
            uint8_t L;
        } s;
        uint16_t b;
    } HL;
    
    // 16-bit stack pointer
    uint16_t SP;
    
    // 16-bit program counter
    uint16_t PC;

    // 8-bit Flag register
    union {
        struct {
            unsigned char S:1;
            unsigned char Z:1;
            unsigned char :1;
            unsigned char AC:1;
            unsigned char :1;
            unsigned char P:1;
            unsigned char :1;
            unsigned char C:1;
        } bits;
        uint8_t FLAGS;
    } PSW;
};

#endif
