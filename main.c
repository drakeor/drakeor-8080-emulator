#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "cpu.h"
#include "disasm.h"

int main()
{
    struct cpustate cpu;
    init_cpu(&cpu);

    printf("Starting..\n");

    int res = disassemble(prom, PROGRAM_SIZE);
    if(res)
       printf("FAILED to process ROM!\n");
 
    return 0;
}
