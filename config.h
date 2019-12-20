#ifndef CONFIG_H
#define CONFIG_H

/*
 * Memory variables
 * REMEMBER to change test variables BELOW if MODIFYING THESE
 */

// Stack pointer start. We're setting it at the end of the memory
#define STACK_START 0x2400

// Memory size, we're going to have 16KB of ram installed.
// We're going to use 8KB for ROM and 8KB for RAM since this is what Space Invaders uses
// 8080 Processors can address up to 64KB though.
#define MEMORY_SIZE 0x4000

// Point to the start of the program here
#define PROGRAM_START 0x00

// Pointer to the VRAM start. Starts at the same pointer as the stack pointer.
// This ASSUMES that VRAM makes up end of stack to end of memory. 
// Setting it to 0x0000 will make the emulator think all memory is VRAM.
#define VRAM_START 0x2400
#define VRAM_DIM_X 256
#define VRAM_DIM_Y 224

// Whether or not we want to dump the VRAM to a BMP on panic
#define DUMP_VRAM_ON_PANIC 1

/*
 * Variables for testing
 */

// Access to the ROM area.
// Set this within the bounds of TEST_ROM_SIZE
// or some tests will fail
#define TEST_MEMORY_ROM_L 0x30
#define TEST_MEMORY_ROM_H 0x10
#define TEST_MEMORY_ROM_HL ((TEST_MEMORY_ROM_H << 8) | TEST_MEMORY_ROM_L)
#define TEST_MEMORY_ROM_LH ((TEST_MEMORY_ROM_L << 8) | TEST_MEMORY_ROM_H)

// Access tests with low and high part of memory.
// Set these within bounds of TEST_MEMORY_SIZE and outside the bounds of TEST_ROM_SIZE
// or some tests will fail
#define TEST_MEMORY_RAM_L 0xBB
#define TEST_MEMORY_RAM_H 0x20
#define TEST_MEMORY_RAM_HL ((TEST_MEMORY_RAM_H << 8) | TEST_MEMORY_RAM_L)
#define TEST_MEMORY_RAM_LH ((TEST_MEMORY_RAM_L << 8) | TEST_MEMORY_RAM_H)

// Access to out of bounds area of memory.
// Set this outside of TEST_MEMORY_SIZE
// or some tests will fail
#define TEST_MEMORY_OOB_L 0xFF
#define TEST_MEMORY_OOB_H 0xFF
#define TEST_MEMORY_OOB_HL ((TEST_MEMORY_OOB_H << 8) | TEST_MEMORY_OOB_L)
#define TEST_MEMORY_OOB_LH ((TEST_MEMORY_OOB_L << 8) | TEST_MEMORY_OOB_H)

// Test memory to sample for tests
#define TEST_MEMORY_BYTE 0xAB
#define TEST_MEMORY_BYTE_2 0xCD
#define TEST_MEMORY_WORD 0xABCD

/*
 * Set to point to the rom here
 */
//#include "roms/blank.h"
#include "roms/invaders.h"

#endif
