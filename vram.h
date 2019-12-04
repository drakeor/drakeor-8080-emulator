#ifndef _VRAM
#define _VRAM

// Standard Libraries
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Include our config file as always
#include "config.h"

/*
 * Dumps vram to a bmp file.
 */
int vram_to_bmp(uint8_t* memory, uint16_t memory_size);

#endif
