#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "config.h"
#include "vram.h"
#include "cpu.h"
#include "disasm.h"

int main()
{
    // Set up CPU
    struct cpustate cpu;
    init_cpu(&cpu);

    printf("Starting..\n");

    unsigned int instruction_count = 1;
    unsigned int max_instruction_count = 0xFFFFFF;

    // Main program loop
    while(!process_cpu(&cpu, prom, MEMORY_SIZE))
    {
        // Step by step after htiting max instruction count
        if(instruction_count >= max_instruction_count) {
            getchar();
            dump_registers(&cpu);
        }

        // Need to copy the PC since op_to_name advances it.
        int tPC = cpu.PC;
        printf("%u - ", instruction_count);
        op_to_text(prom, MEMORY_SIZE, &tPC);
        ++instruction_count;
    }

    // Dump final state of the CPU and VRAM
    dump_registers(&cpu);
    if(DUMP_VRAM_ON_PANIC) { 
        vram_to_bmp(); 
        printf("vram dumped to file.\n"); 
    }

    return 0;
}
