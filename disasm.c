#include <stdio.h>
#include "disasm.h"

#define CHECK_BUFFER(x) { if((*counter)+x >= buffer_size) { printf("\nbuffer overflow\n"); return -2; }};

// REMEMBER MEMORY ALIGNMENT!
#define PRINT_WORD(x) { CHECK_BUFFER(2); printf("%s0x%02X%02X", x, buffer[(*counter)+2], buffer[(*counter)+1]); *counter += 3; };

#define PRINT_BYTE(x) { CHECK_BUFFER(1); printf("%s0x%02X", x, buffer[(*counter)+1]);  *counter += 2; };

#define PRINT_OP(x) { printf("%s", x); *counter += 1; }

int op_to_text(unsigned char* buffer, int buffer_size, int* counter)
{
    int res = 0;
    CHECK_BUFFER(0);

    printf("%04X: %02X ", *counter, buffer[*counter]);
    switch(buffer[*counter]) {
        case 0x00: PRINT_OP("NOP"); break;

        //LD bytes to registers 
        case 0x3E: PRINT_BYTE("LD A,"); break;
        case 0x06: PRINT_BYTE("LD B,"); break;
        case 0x0E: PRINT_BYTE("LD C,"); break;
        case 0x16: PRINT_BYTE("LD D,"); break;
        case 0x1E: PRINT_BYTE("LD E,"); break;
        case 0x26: PRINT_BYTE("LD H,"); break;
        case 0x2E: PRINT_BYTE("LD L,"); break;

        // LXI - words to registers 
        case 0x01: PRINT_WORD("LX BC,"); break;
        case 0x11: PRINT_WORD("LX DE,"); break;
        case 0x21: PRINT_WORD("LX HL,"); break;
        case 0x31: PRINT_WORD("LX SP,"); break;

        // CALL - calls to addresses
        case 0xCD: PRINT_WORD("CALL "); break;
        case 0xC4: PRINT_WORD("CNZ,"); break;
        case 0xCC: PRINT_WORD("CZ,"); break;
        case 0xD4: PRINT_WORD("CNC,"); break;
        case 0xDC: PRINT_WORD("CC,"); break;
        case 0xE4: PRINT_WORD("CPO,"); break;
        case 0xEC: PRINT_WORD("CPE,"); break;
        case 0xF4: PRINT_WORD("CP,"); break;
        case 0xFC: PRINT_WORD("CM,"); break;

        // MOV - Transfer from memory to register
        case 0x46: PRINT_OP("MOV B,(HL)"); break;
        case 0x4E: PRINT_OP("MOV C,(HL)"); break;
        case 0x56: PRINT_OP("MOV D,(HL)"); break;
        case 0x5E: PRINT_OP("MOV E,(HL)"); break;
        case 0x66: PRINT_OP("MOV H,(HL)"); break;
        case 0x6E: PRINT_OP("MOV L,(HL)"); break;
        case 0x7E: PRINT_OP("MOV A,(HL)"); break;

        // MOV - Transfer from memory to A
        case 0x0A: PRINT_OP("MOV A,(BC)"); break;
        case 0x1A: PRINT_OP("MOV A,(DE)"); break;

         // MOV - Transfer register to memory
        case 0x77: PRINT_OP("MOV (HL),A"); break;
        case 0x70: PRINT_OP("MOV (HL),B"); break;
        case 0x71: PRINT_OP("MOV (HL),C"); break;
        case 0x72: PRINT_OP("MOV (HL),D"); break;
        case 0x73: PRINT_OP("MOV (HL),E"); break;
        case 0x74: PRINT_OP("MOV (HL),H"); break;
        case 0x75: PRINT_OP("MOV (HL),L"); break;

        // INC - Increment register (byte)
        case 0x3C: PRINT_OP("INC A"); break;
        case 0x04: PRINT_OP("INC B"); break;
        case 0x0C: PRINT_OP("INC C"); break;
        case 0x14: PRINT_OP("INC D"); break;
        case 0x1C: PRINT_OP("INC E"); break;
        case 0x24: PRINT_OP("INC H"); break;
        case 0x2C: PRINT_OP("INC L"); break;
        case 0x34: PRINT_OP("INC (HL)"); break;

        // DEC - Decrement register (byte)
        case 0x3D: PRINT_OP("DEC A"); break;
        case 0x05: PRINT_OP("DEC B"); break;
        case 0x0D: PRINT_OP("DEC C"); break;
        case 0x15: PRINT_OP("DEC D"); break;
        case 0x1D: PRINT_OP("DEC E"); break;
        case 0x25: PRINT_OP("DEC H"); break;
        case 0x2D: PRINT_OP("DEC L"); break;
        case 0x35: PRINT_OP("DEC (HL)"); break;

        // INC - Increment register pair
        case 0x03: PRINT_OP("INC BC"); break;
        case 0x13: PRINT_OP("INC DE"); break;
        case 0x23: PRINT_OP("INC HL"); break;
        case 0x33: PRINT_OP("INC SP"); break;

        // DEC - Decrement register pair
        case 0x0B: PRINT_OP("DEC BC"); break;
        case 0x1B: PRINT_OP("DEC DE"); break;
        case 0x2B: PRINT_OP("DEC HL"); break;
        case 0x3B: PRINT_OP("DEC SP"); break;
        
        // JMP - goto new pc
        case 0xC3: PRINT_WORD("JMP,"); break;
        case 0xC2: PRINT_WORD("JNZ,"); break;
        case 0xCA: PRINT_WORD("JZ,"); break;
        case 0xD2: PRINT_WORD("JNC,"); break;
        case 0xDA: PRINT_WORD("JC,"); break;
        case 0xE2: PRINT_WORD("JPO,"); break;
        case 0xEA: PRINT_WORD("JPE,"); break;
        case 0xF2: PRINT_WORD("JP,"); break;
        case 0xFA: PRINT_WORD("JM,"); break;

        // RET Functions
        case 0xc9: PRINT_OP("RET"); break;

        // MVI Functions
        case 0x36: PRINT_BYTE("MVI (HL),"); break;

        // MOV B, <REG> Instructions
        case 0x40: PRINT_OP("MOV B,B"); break;
        case 0x41: PRINT_OP("MOV B,C"); break;
        case 0x42: PRINT_OP("MOV B,D"); break;
        case 0x43: PRINT_OP("MOV B,E"); break;
        case 0x44: PRINT_OP("MOV B,H"); break;
        case 0x45: PRINT_OP("MOV B,L"); break;
        case 0x47: PRINT_OP("MOV B,A"); break;

        // MOV C, <REG> Instructions
        case 0x48: PRINT_OP("MOV C,B"); break;
        case 0x49: PRINT_OP("MOV C,C"); break;
        case 0x4a: PRINT_OP("MOV C,D"); break;
        case 0x4b: PRINT_OP("MOV C,E"); break;
        case 0x4c: PRINT_OP("MOV C,H"); break;
        case 0x4d: PRINT_OP("MOV C,L"); break;
        case 0x4f: PRINT_OP("MOV C,A"); break;

        // MOV D, <REG> Instructions
        case 0x50: PRINT_OP("MOV D,B"); break;
        case 0x51: PRINT_OP("MOV D,C"); break;
        case 0x52: PRINT_OP("MOV D,D"); break;
        case 0x53: PRINT_OP("MOV D,E"); break;
        case 0x54: PRINT_OP("MOV D,H"); break;
        case 0x55: PRINT_OP("MOV D,L"); break;
        case 0x57: PRINT_OP("MOV D,A"); break;

        // MOV E, <REG> Instructions
        case 0x58: PRINT_OP("MOV E,B"); break;
        case 0x59: PRINT_OP("MOV E,C"); break;
        case 0x5a: PRINT_OP("MOV E,D"); break;
        case 0x5b: PRINT_OP("MOV E,E"); break;
        case 0x5c: PRINT_OP("MOV E,H"); break;
        case 0x5d: PRINT_OP("MOV E,L"); break;
        case 0x5f: PRINT_OP("MOV E,A"); break;

        // MOV H, <REG> Instructions
        case 0x60: PRINT_OP("MOV H,B"); break;
        case 0x61: PRINT_OP("MOV H,C"); break;
        case 0x62: PRINT_OP("MOV H,D"); break;
        case 0x63: PRINT_OP("MOV H,E"); break;
        case 0x64: PRINT_OP("MOV H,H"); break;
        case 0x65: PRINT_OP("MOV H,L"); break;
        case 0x67: PRINT_OP("MOV H,A"); break;

        // MOV L, <REG> Instructions
        case 0x68: PRINT_OP("MOV L,B"); break;
        case 0x69: PRINT_OP("MOV L,C"); break;
        case 0x6a: PRINT_OP("MOV L,D"); break;
        case 0x6b: PRINT_OP("MOV L,E"); break;
        case 0x6c: PRINT_OP("MOV L,H"); break;
        case 0x6d: PRINT_OP("MOV L,L"); break;
        case 0x6f: PRINT_OP("MOV L,A"); break;

        // MOV A, <REG> Instructions
        case 0x78: PRINT_OP("MOV A,B"); break;
        case 0x79: PRINT_OP("MOV A,C"); break;
        case 0x7a: PRINT_OP("MOV A,D"); break;
        case 0x7b: PRINT_OP("MOV A,E"); break;
        case 0x7c: PRINT_OP("MOV A,H"); break;
        case 0x7d: PRINT_OP("MOV A,L"); break;
        case 0x7f: PRINT_OP("MOV A,A"); break;

        // CPI
        case 0xFE: PRINT_BYTE("CPI,"); break;

        // PUSH instructions
        case 0xC5: PRINT_OP("PUSH B"); break;
        case 0xD5: PRINT_OP("PUSH D"); break;
        case 0xE5: PRINT_OP("PUSH E"); break;
        case 0xF5: PRINT_OP("PUSH PSW"); break;

        // DAD Instructions
        case 0x09: PRINT_OP("DAD B"); break;
        case 0x19: PRINT_OP("DAD D"); break;
        case 0x29: PRINT_OP("DAD H"); break;
        case 0x39: PRINT_OP("DAD SP"); break;

        // XCHG instruction
        case 0xEB: PRINT_OP("XCHG"); break;

        // POP instructions
        case 0xC1: PRINT_OP("POP B"); break;
        case 0xD1: PRINT_OP("POP D"); break;
        case 0xE1: PRINT_OP("POP H"); break;
        case 0xC6: PRINT_OP("POP PSW"); break;

        // Register Rotations
        case 0x07: PRINT_OP("RLC"); break;
        case 0x0F: PRINT_OP("RRC"); break;

        // Accumulator Bitwise Operators
        case 0xE6: PRINT_BYTE("ANI,"); break;
        case 0xEE: PRINT_BYTE("XRI,"); break;
        case 0xF6: PRINT_BYTE("ORI,"); break;
        case 0xF1: PRINT_BYTE("ADI,"); break;

        // Store/Load Accumulator/Memory
        case 0x3A: PRINT_WORD("LDA,"); break;
        case 0x32: PRINT_WORD("STA,"); break;

        // Bitwise operations
        case 0xA0: PRINT_OP("ANA B"); break;
        case 0xA1: PRINT_OP("ANA C"); break;
        case 0xA2: PRINT_OP("ANA D"); break;
        case 0xA3: PRINT_OP("ANA E"); break;
        case 0xA4: PRINT_OP("ANA H"); break;
        case 0xA5: PRINT_OP("ANA L"); break;
        case 0xA6: PRINT_OP("ANA M"); break;
        case 0xA7: PRINT_OP("ANA A"); break;
        case 0xA8: PRINT_OP("XRA B"); break;
        case 0xA9: PRINT_OP("XRA C"); break;
        case 0xAA: PRINT_OP("XRA D"); break;
        case 0xAB: PRINT_OP("XRA E"); break;
        case 0xAC: PRINT_OP("XRA H"); break;
        case 0xAD: PRINT_OP("XRA L"); break;
        case 0xAE: PRINT_OP("XRA M"); break;
        case 0xAF: PRINT_OP("ANA A"); break;
        case 0xB0: PRINT_OP("ORA B"); break;
        case 0xB1: PRINT_OP("ORA C"); break;
        case 0xB2: PRINT_OP("ORA D"); break;
        case 0xB3: PRINT_OP("ORA E"); break;
        case 0xB4: PRINT_OP("ORA H"); break;
        case 0xB5: PRINT_OP("ORA L"); break;
        case 0xB6: PRINT_OP("ORA M"); break;
        case 0xB7: PRINT_OP("ORA A"); break;

        // Interrupts
        case 0xFB: PRINT_OP("EI"); break;
        case 0xF3: PRINT_OP("DI"); break;

        // Unknown
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

