#include <stdio.h>
#include "disasm.h"
#define CHECK_BUFFER(x) { if((*counter)+x >= buffer_size) { printf("\nbuffer overflow\n"); return -2; }};

int op_to_text(unsigned char* buffer, int buffer_size, int* counter)
{
    int res = 0;
    CHECK_BUFFER(0);

    printf("%04X: %02X ", *counter, buffer[*counter]);
    switch(buffer[*counter]) {
        case 0x00:
            printf("NOP");
            *counter += 1;
            break;
        case 0xC3:
            CHECK_BUFFER(2);
            printf("JMP 0x%02X%02X", buffer[(*counter)+1], buffer[(*counter)+2]);
            *counter += 1;
            break;
        default:
            printf("???");
            *counter += 1;
            res = -1;
    }
    printf("\n");
    return res;
}

int disassemble(unsigned char* buffer, int buffer_size)
{
    int pc = 0x00; 
    while(pc < buffer_size) {
        if(op_to_text(buffer, buffer_size, &pc) < 0)
            return -1; 
    }
   return 0;
}

