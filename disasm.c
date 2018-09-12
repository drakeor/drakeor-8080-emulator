#include <stdio.h>
#include "disasm.h"

#define CHECK_BUFFER(x) { if((*counter)+x >= buffer_size) { printf("\nbuffer overflow\n"); return -2; }};

#define PRINT_WORD(x) { CHECK_BUFFER(2); printf("%s 0x%02X%02X", x, buffer[(*counter)+1], buffer[(*counter)+2]); *counter += 3; };

#define PRINT_BYTE(x) { CHECK_BUFFER(1); printf("%s 0x%02X", x, buffer[(*counter)+1]);  *counter += 2; };

#define PRINT_OP(x) { printf("%s", x); *counter += 1; }

int op_to_text(unsigned char* buffer, int buffer_size, int* counter)
{
    int res = 0;
    CHECK_BUFFER(0);

    printf("%04X: %02X ", *counter, buffer[*counter]);
    switch(buffer[*counter]) {
        case 0x00: PRINT_OP("NOP"); break;

        case 0x3E: PRINT_BYTE("LD A,"); break;
        case 0x06: PRINT_BYTE("LD B,"); break;
        case 0x0E: PRINT_BYTE("LD C,"); break;
        case 0x16: PRINT_BYTE("LD D,"); break;
        case 0x1E: PRINT_BYTE("LD E,"); break;
        case 0x26: PRINT_BYTE("LD H,"); break;
        case 0x2E: PRINT_BYTE("LD L,"); break;

        case 0x31: PRINT_WORD("LX SP,"); break;
        case 0xC3: PRINT_WORD("JMP"); break;
        default:
            PRINT_OP("???");
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

