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


        /*
         * MOV from memory to A 
         */

        // 0x7E - Load HL to A
        case 0x7E:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            cpu->A = memory[cpu->HL]; 
            cpu->PC += 1;
            break;

        // 0x7E - Load BC to A
        case 0x0A:
            if(cpu->BC >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            cpu->A = memory[cpu->BC]; 
            cpu->PC += 1;
            break;

        // 0x7E - Load DE to A
        case 0x1A:
            if(cpu->DE >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            cpu->A = memory[cpu->DE]; 
            cpu->PC += 1;
            break;

        /*
        * MOV from register to memory
        */

        // 0x77 - MOV A to memory
        case 0x77:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->A; 
            cpu->PC += 1;
            break;

        // 0x70 - MOV B to memory
        case 0x70:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->B; 
            cpu->PC += 1;
            break;

        // 0x71 - MOV C to memory
        case 0x71:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->C; 
            cpu->PC += 1;
            break;

        // 0x72 - MOV D to memory        
        case 0x72:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->D; 
            cpu->PC += 1;
            break;

        // 0x73 - MOV E to memory                
        case 0x73:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->E; 
            cpu->PC += 1;
            break;

        // 0x74 - MOV H to memory 
        case 0x74:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->H; 
            cpu->PC += 1;
            break;

        // 0x75 - MOV L to memory 
        case 0x75:
            if(cpu->HL >= memory_size)
                 PANIC("attempting to access memory out of bounds");
            memory[cpu->HL] =  cpu->L; 
            cpu->PC += 1;
            break;

        /*
        * INC byte
        */

        // 0x3C - INC A 
        case 0x3C:
            if(cpu->A == 0xFF)
                cpu->A = 0x00;
            else
                cpu->A += 1;
            cpu->PC += 1;
            break;

        // 0x04 - INC B
        case 0x04:
            if(cpu->B == 0xFF)
                cpu->B = 0x00;
            else
                cpu->B += 1;
            cpu->PC += 1;
            break;

        // 0x0C - INC C 
        case 0x0C:
            if(cpu->C == 0xFF)
                cpu->C = 0x00;
            else
                cpu->C += 1;
            cpu->PC += 1;
            break;

        // 0x14 - INC D 
        case 0x14:
            if(cpu->D == 0xFF)
                cpu->D = 0x00;
            else
                cpu->D += 1;
            cpu->PC += 1;
            break;

        // 0x1C - INC E 
        case 0x1C:
           if(cpu->E == 0xFF)
                cpu->E = 0x00;
            else
                cpu->E += 1;
            cpu->PC += 1;
            break;

        // 0x24 - INC H
        case 0x24:
            if(cpu->H == 0xFF)
                cpu->H = 0x00;
            else
                cpu->H += 1;
            cpu->PC += 1;
            break;
            
        // 0x2C - INC L
        case 0x2C:
            if(cpu->L == 0xFF)
                cpu->L = 0x00;
            else
                cpu->L += 1;
            cpu->PC += 1;
            break;
            
        // 0x34 - INC (HL)
        case 0x34:
            if(cpu->HL >= memory_size)
                PANIC("attempting to access memory out of bounds");
            if((cpu->HL - 2) < ROM_SIZE)
                PANIC("0x34 instruction will write into ROM");
            if(memory[cpu->HL] == 0xFF)
                memory[cpu->HL] = 0x00;
            else
                memory[cpu->HL] += 1;
            cpu->PC += 1;
            break;

        /*
        * DEC byte
        */

        // 0x3D - DEC A 
        case 0x3D:
            if(cpu->A == 0x00)
                cpu->A = 0xFF;
            else
                cpu->A -= 1;
            cpu->PC += 1;
            break;

        // 0x05 - DEC B
        case 0x05:
            if(cpu->B == 0x00)
                cpu->B = 0xFF;
            else
                cpu->B -= 1;
            cpu->PC += 1;
            break;

        // 0x0D - DEC C
        case 0x0D:
            if(cpu->C == 0x00)
                cpu->C = 0xFF;
            else
                cpu->C -= 1;
            cpu->PC += 1;
            break;

        // 0x15 - DEC D
        case 0x15:
            if(cpu->D == 0x00)
                cpu->D = 0xFF;
            else
                cpu->D -= 1;
            cpu->PC += 1;
            break;

        // 0x1D - DEC E 
        case 0x1D:
           if(cpu->E == 0x00)
                cpu->E = 0xFF;
            else
                cpu->E -= 1;
            cpu->PC += 1;
            break;

        // 0x25 - DEC H
        case 0x25:
           if(cpu->H == 0x00)
                cpu->H = 0xFF;
            else
                cpu->H -= 1;
            cpu->PC += 1;
            break;
            
        // 0x2D - DEC L
        case 0x2D:
           if(cpu->L == 0x00)
                cpu->L = 0xFF;
            else
                cpu->L -= 1;
            cpu->PC += 1;
            break;
            
        // 0x34 - DEC (HL)
        case 0x35:
            if(cpu->HL >= memory_size)
                PANIC("attempting to access memory out of bounds");
            if((cpu->HL - 2) < ROM_SIZE)
                PANIC("0x35 instruction will write into ROM");
            if(memory[cpu->HL] == 0x00)
                memory[cpu->HL] = 0xFF;
            else
                memory[cpu->HL] -= 1;
            cpu->PC += 1;
            break;

        /*
        * INC register pair
        */

       // 0x03 - INC BC
       case 0x03:
            if(cpu->BC == 0xFFFF)
                cpu->BC = 0x0000;
            else
                cpu->BC += 1;
            cpu->PC += 1;
            break;

       // 0x13 - INC DE        
        case 0x13:
            if(cpu->DE == 0xFFFF)
                cpu->DE = 0x0000;
            else
                cpu->DE += 1;
            cpu->PC += 1;
            break;

       // 0x23 - INC HL        
        case 0x23:
            if(cpu->HL == 0xFFFF)
                cpu->HL = 0x0000;
            else
                cpu->HL += 1;
            cpu->PC += 1;
            break;

       // 0x33 - INC SP        
        case 0x33:
            if(cpu->SP == 0xFFFF)
                cpu->SP = 0x0000;
            else
                cpu->SP += 1;
            cpu->PC += 1;
            break;

        /*
        * DEC register pair
        */

       // 0x0B - DEC BC
       case 0x0B:
            if(cpu->BC == 0x0000)
                cpu->BC = 0xFFFF;
            else
                cpu->BC -= 1;
            cpu->PC += 1;
            break;

       // 0x1B - DEC DE
       case 0x1B:
            if(cpu->DE == 0x0000)
                cpu->DE = 0xFFFF;
            else
                cpu->DE -= 1;
            cpu->PC += 1;
            break;

       // 0x2B - DEC HL
       case 0x2B:
            if(cpu->HL == 0x0000)
                cpu->HL = 0xFFFF;
            else
                cpu->HL -= 1;
            cpu->PC += 1;
            break;

       // 0x3B - DEC SP
       case 0x3B:
            if(cpu->SP == 0x0000)
                cpu->SP = 0xFFFF;
            else
                cpu->SP -= 1;
            cpu->PC += 1;
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
