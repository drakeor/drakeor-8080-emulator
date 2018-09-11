#ifndef CPU_H__
#define CPU_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Load config
#include "config.h"

// Stack pointer start
#define STACK_START 0xF000

// Holds the current state of the CPU
struct cpustate {
    
    // A is the primary 8-bit accumulator
    uint8_t A;
    
    // 3 16-bit registers (BC, DE, HL)
    // that can function as 6 8-bit registers (B, C, D, E, H, L)
    union {
        struct {
            uint8_t C;
            uint8_t B;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t E;
            uint8_t D;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t L;
            uint8_t H;
        };
        uint16_t HL;
    };
    
    // 16-bit stack pointer
    uint16_t SP;
    
    // 16-bit program counter
    uint16_t PC;

    // 8-bit Flag register
    union {
        struct {
            unsigned char C:1;  // Carry, set if the last addition operation resulted in a carry 
                                // or last sub required borrow
            unsigned char :1;
            unsigned char P:1;  // Parity bit, set if the number of true bits in the result is even
            unsigned char :1;
            unsigned char AC:1; // Auxiliary carry bit for binary coded decimal arithmetic 
            unsigned char :1;   
            unsigned char Z:1;  // Zero bit, set if the result is zero 
            unsigned char S:1;  // Sign bit, set if the result is negative
        } FLAGS;
        uint8_t PSW;
    };
};


// Returns a new cpu instance
int init_cpu(struct cpustate* cpu);
#endif



