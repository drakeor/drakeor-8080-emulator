#ifndef CONFIG_H
#define CONFIG_H

// Stack pointer start. We're setting it at the end of the memory
#define STACK_START 0x4000

// Memory size, we're going to have 16KB of ram installed.
// We're going to use 8KB for ROM and 8KB for RAM since this is what Space Invaders uses
// 8080 Processors can address up to 64KB though.
#define MEMORY_SIZE 0x4000

// Point to the start of the program here
#define PROGRAM_START 0x00

// Set to 1 to write-protect ROM segment.
// No memory in the loaded part of the rom can be written to.
#define ROM_WRITEPROTECT 1

// Set to point to the rom here
//#include "roms/blank.h"
#include "roms/invaders.h"

#endif
