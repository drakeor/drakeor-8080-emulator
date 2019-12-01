#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "config.h"
#include "cpu.h"
#include "disasm.h"

int main()
{
    // Set up CPU
    struct cpustate cpu;
    init_cpu(&cpu);

    printf("Starting..\n");

    unsigned int instruction_count = 1;

    // Main program loop
    do {
        // Need to copy the PC since op_to_name advances it.
        int tPC = cpu.PC;
        printf("%u - ", instruction_count);
        op_to_text(prom, MEMORY_SIZE, &tPC);
        ++instruction_count;

    } while(!process_cpu(&cpu, prom, MEMORY_SIZE));

    return 0;
}
