#include <stdio.h>
#include "disasm.h"

int op_to_text(unsigned char* buffer, int* counter)
{
    int res = 0;
    printf("%04X: %02X ", *counter, buffer[*counter]);
    switch(buffer[*counter]) {
        case 0x00:
            printf("NOP");
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
       if(op_to_text(buffer, &pc) < 0)
            return -1; 
   }
   return 0;
}

