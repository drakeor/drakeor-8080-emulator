#include "cpu.h"

// Helper macro for panic
#define PANIC(...) { printf("\npanic: "); printf(__VA_ARGS__); dump_registers(cpu); return -1; }

// Helper to check if we have enough program space
#define CHECK_BUFFER(x) { if(cpu->PC+x >= prom_size) PANIC("%02X instruction overflows buffer", cpu->PC); }

// Populates x with the next byte of information
#define GET_BYTE(x) { CHECK_BUFFER(1); x = prom[cpu->PC+1];  };

// Populates x with the next word of information with proper memory alignment.
#define GET_WORD(x) { CHECK_BUFFER(2); x = prom[cpu->PC+1] + (prom[cpu->PC+2] << 8);  };

int init_cpu(struct cpustate* cpu) {
    // Set the PC register to PROGRAM_START
    // Set SP to start at 0xF000
    cpu->PC = PROGRAM_START; 
    cpu->SP = STACK_START;

    // Zero out the registers
    cpu->A = 0;
    cpu->BC = 0;
    cpu->DE = 0;
    cpu->HL = 0;
    cpu->PSW = 0;

    return 0;
}

// Process a CPU instruction
int process_cpu(struct cpustate* cpu, unsigned char* prom, int prom_size)
{
    // Sanity check
    if(cpu->PC >= prom_size) {
        PANIC("pc counter overflowed");
    }
    
    // Temp 16-bit register
    uint16_t tmp;
    tmp = 0;

    switch(prom[cpu->PC]) {
        
        // 0x00 = NOP
        case 0x00:
            cpu->PC += 1;
            break;

        /* 
         * LD bytes to registers 
         */

        // LOAD A, byte
        case 0x3E:
            GET_BYTE(cpu->A);
            cpu->PC += 2;
            break;

        // LOAD B, byte
        case 0x06:
            GET_BYTE(cpu->B);
            cpu->PC += 2;
            break;

        // LOAD C, byte
        case 0x0E:
            GET_BYTE(cpu->C);
            cpu->PC += 2;
            break;

        // LOAD D, byte
        case 0x16:
            GET_BYTE(cpu->D);
            cpu->PC += 2;
            break;

        // LOAD E, byte
        case 0x1E:
            GET_BYTE(cpu->E);
            cpu->PC += 2;
            break;

        // LOAD H, byte
        case 0x26:
            GET_BYTE(cpu->H);
            cpu->PC += 2;
            break;

        // LOAD L, byte
        case 0x2E:
            GET_BYTE(cpu->L);
            cpu->PC += 2;
            break;


        /* 
         * LXI - words to registers 
         */
        // 0x01 = LXI BC, word
        case 0x01:
            GET_WORD(cpu->BC);
            cpu->PC += 3;
            break;

        // 0x11 = LXI DE, word
        case 0x11:
            GET_WORD(cpu->DE);
            cpu->PC += 3;
            break;

        // 0x21 = LXI HL, word
        case 0x21:
            GET_WORD(cpu->HL);
            cpu->PC += 3;
            break;

        // 0x31 = LXI SP, word
        case 0x31:
            GET_WORD(cpu->SP);
            cpu->PC += 3;
            break;

        /*
         * Jumps
         */

        // 0xC3 = JMP 0x0000 
        case 0xC3:
            GET_WORD(tmp);
            if(tmp >= prom_size)
                PANIC("C3 instruction jumped outside memory bounds");
            cpu->PC = tmp;
            break;

        /*
         * Calls
         */

        // 0xCD = CALL addr 
        case 0xCD:
            GET_WORD(tmp);
            if(tmp >= prom_size)
                PANIC("CD instruction jumped outside memory bounds");
            if(cpu->SP < 2)
                PANIC("CD instruction will underflow");
            prom[cpu->SP - 1] = (0xFF00 & cpu->PC) >> 8;
            prom[cpu->SP - 2] = (0xFF & cpu->PC);
            cpu->SP -= 2;
            cpu->PC = tmp; 
            break;
        

        // Panic if we don't know the instruction
        default:
            printf("Cannot process opcode %02X\n", prom[cpu->PC]);
            PANIC("opcode not implemented");
    }
    return 0;
}

int dump_registers(struct cpustate* cpu)
{
    printf("\nPC: 0x%04X | SP: 0x%04X\n", cpu->PC, cpu->SP);
    printf("A: 0x%02X | BC: 0x%04X | DE: 0x%04X | HL: 0x%04X\n", 
        cpu->A, cpu->BC, cpu->DE, cpu->HL); 
    printf("F-C: %u | F-P: %u | F-AC: %u | F-Z: %u | F-S: %u\n",
        cpu->FLAGS.C, cpu->FLAGS.P, cpu->FLAGS.AC, cpu->FLAGS.Z, cpu->FLAGS.S);
}
