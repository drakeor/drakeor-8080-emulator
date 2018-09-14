#ifndef CPU_H__
#define CPU_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Load config
#include "config.h"

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
            uint8_t C:1;  // Carry, set if the last addition operation resulted in a carry 
                                // or last sub required borrow
            uint8_t :1;
            uint8_t P:1;  // Parity bit, set if the number of true bits in the result is even
            uint8_t :1;
            uint8_t AC:1; // Auxiliary carry bit for binary coded decimal arithmetic 
            uint8_t :1;   
            uint8_t Z:1;  // Zero bit, set if the result is zero 
            uint8_t S:1;  // Sign bit, set if the result is negative
        } FLAGS;
        uint8_t PSW;
    };
};


// Returns a new cpu instance
int init_cpu(struct cpustate* cpu);
int process_cpu(struct cpustate* cpu, uint8_t* memory, uint16_t memory_size);
int dump_registers(struct cpustate* cpu);

#endif
