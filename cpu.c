#include "cpu.h"

/*
 * Helper macros
 */
// Helper macro for panic
#define PANIC(...) { printf("\npanic: "); printf(__VA_ARGS__); dump_registers(cpu); return -1; }
// Helper to check if we have enough program space
#define CHECK_BUFFER(x) { if(cpu->PC+x >= memory_size) PANIC("%02X instruction overflows buffer", cpu->PC); }
// Populates x with the next byte of information
#define GET_BYTE(x) { CHECK_BUFFER(1); x = memory[cpu->PC+1];  };
// Populates x with the next word of information with proper memory alignment.
#define GET_WORD(x) { CHECK_BUFFER(2); x = memory[cpu->PC+1] + (memory[cpu->PC+2] << 8);  };


/*
 * Helper functions
 */

// Helper function for call instructions
int do_call_inst(struct cpustate* cpu, uint8_t opcode, uint8_t* memory, uint16_t memory_size)
{
    uint16_t tmp;
    GET_WORD(tmp);
    if(tmp >= memory_size)
        PANIC("%02X instruction jumped outside memory bounds", opcode);
    if(cpu->SP > memory_size)
        PANIC("%02X instruction has SP that overflows memory bounds", opcode);
    if(cpu->SP < 2)
        PANIC("%02X instruction will underflow stack pointer", opcode);
    if((cpu->SP - 2) < ROM_SIZE)
        PANIC("%02X instruction will write into ROM", opcode);
    memory[cpu->SP - 1] = (0xFF & cpu->PC);
    memory[cpu->SP - 2] = cpu->PC >> 8;
    cpu->SP -= 2;
    cpu->PC = tmp; 

    return 0;
}

// Resets the CPU
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
int process_cpu(struct cpustate* cpu, uint8_t* memory, uint16_t memory_size)
{
    // Sanity check
    if(cpu->PC >= memory_size) {
        PANIC("pc counter overflowed");
    }
    
    // Temp 16-bit register
    uint16_t tmp;
    tmp = 0;

    switch(memory[cpu->PC]) {
        
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
            if(tmp >= memory_size)
                PANIC("C3 instruction jumped outside memory bounds");
            cpu->PC = tmp;
            break;

        /*
         * Calls
         */
        
        // 0xCD = CALL addr 
        case 0xCD:
            return do_call_inst(cpu, 0xCD, memory, memory_size);
            break;

        // 0xC4 = CALL addr if Z is 1
        case 0xC4:
            CHECK_BUFFER(2);
            if(cpu->FLAGS.Z)
                return do_call_inst(cpu, 0xC4, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xCC = CALL addr if Z is 0
        case 0xCC:
            CHECK_BUFFER(2);
            if(!cpu->FLAGS.Z)
                return do_call_inst(cpu, 0xCC, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xD4 = CALL addr if C is 0
        case 0xD4:
            CHECK_BUFFER(2);
            if(!cpu->FLAGS.C)
                return do_call_inst(cpu, 0xD4, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xDC = CALL addr if C is 1
        case 0xDC:
            CHECK_BUFFER(2);
            if(cpu->FLAGS.C)
                return do_call_inst(cpu, 0xDC, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xE4 = CALL addr if P is 0
        case 0xE4:
            CHECK_BUFFER(2);
            if(!cpu->FLAGS.P)
                return do_call_inst(cpu, 0xE4, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xEC = CALL addr if P is 1
        case 0xEC:
            CHECK_BUFFER(2);
            if(cpu->FLAGS.P)
                return do_call_inst(cpu, 0xEC, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xF4 = CALL addr if S is 0
        case 0xF4:
            CHECK_BUFFER(2);
            if(!cpu->FLAGS.S)
                return do_call_inst(cpu, 0xF4, memory, memory_size);
            cpu->PC += 3;
            break;

        // 0xFC = CALL addr if S is 1
        case 0xFC:
            CHECK_BUFFER(2);
            if(cpu->FLAGS.S)
                return do_call_inst(cpu, 0xFC, memory, memory_size);
            cpu->PC += 3;
            break;

        // Panic if we don't know the instruction
        default:
            printf("Cannot process opcode %02X\n", memory[cpu->PC]);
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
