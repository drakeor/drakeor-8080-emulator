#include "cpu.h"

#define PANIC(...) { printf("\npanic: "); printf(__VA_ARGS__); dump_registers(cpu); return -1; }
#define CHECK_BUFFER(x) { if(cpu->PC+x >= prom_size) PANIC("%02X instruction overflows buffer", cpu->PC); }

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
         * LOAD commands 
         */

        // LOAD A, byte
        case 0x3E:
            CHECK_BUFFER(1);
            cpu->A = prom[cpu->PC+1];
            cpu->PC += 2;
            break;

        // LOAD B, byte
        case 0x06:
            CHECK_BUFFER(1);
            cpu->B = prom[cpu->PC+1];
            cpu->PC += 2;
            break;

        // LOAD C, byte
        case 0x0E:
            CHECK_BUFFER(1);
            cpu->C = prom[cpu->PC+1];
            cpu->PC += 2;
            break;

        // LOAD D, byte
        case 0x16:
            CHECK_BUFFER(1);
            cpu->D = prom[cpu->PC+1];
            cpu->PC += 2;
            break;

        // LOAD E, byte
        case 0x1E:
            CHECK_BUFFER(1);
            cpu->E = prom[cpu->PC+1];
            cpu->PC += 2;
            break;

        // LOAD H, byte
        case 0x26:
            CHECK_BUFFER(1);
            cpu->H = prom[cpu->PC+1];
            cpu->PC += 2;
            break;

        // LOAD L, byte
        case 0x2E:
            CHECK_BUFFER(1);
            cpu->L = prom[cpu->PC+1];
            cpu->PC += 2;
            break;


        /* 
         * 16-bit LOAD instructions 
         */

        // 0x31 = LXI SP, word
        case 0x31:
            CHECK_BUFFER(2);
            cpu->SP = prom[cpu->PC+1] + (prom[cpu->PC+2] << 8);
            cpu->PC += 3;
            break;

        // 0xC3 = JMP 0x0000 
        case 0xC3:
            CHECK_BUFFER(2);
            tmp = prom[cpu->PC+1] 
                + (prom[cpu->PC+2] << 8);
            if(tmp >= prom_size)
                PANIC("C3 instruction jumped outside memory bounds");
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
