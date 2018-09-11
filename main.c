#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "cpu.h"

int main()
{
    struct cpustate cpu;
    init_cpu(&cpu);
    printf("Test\n");
    return 0;
}
