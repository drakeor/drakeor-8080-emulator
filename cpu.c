#include "cpu.h"
#include "vram.h"

/*
 * Helper macros
 */
// Helper macro for panic
#define PANIC(...) { printf("\npanic: "); printf(__VA_ARGS__); dump_registers(cpu); if(DUMP_VRAM_ON_PANIC) { vram_to_bmp(); printf("vram dumped to file."); } return -1; }
// Helper to check if we have enough program space
#define CHECK_BUFFER(x) { if(cpu->PC+x >= memory_size) PANIC("%02X instruction overflows buffer", cpu->PC); }
// Populates x with the next byte of information
#define GET_BYTE(x) { CHECK_BUFFER(1); x = memory[cpu->PC+1];  };
// Populates x with the next word of information with proper memory alignment. REMEMBER MEMORY ALIGNMENT
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

    // We need to store the next instruction after CALL to prevent RET from invoking an infinite loop
    cpu->PC = cpu->PC + 3;
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

    // Temp 32-bit register
    uint32_t tmp32;
    tmp32 = 0;

    // Temp 16-bit register
    uint16_t tmp;
    tmp = 0;

    // Temp 8-bit register
    uint8_t tmp8;
    tmp8 = 0;

    // Reg mapping
    char *reg_mapping[8] = {
        &(cpu->B),
        &(cpu->C),
        &(cpu->D),
        &(cpu->E),
        &(cpu->H),
        &(cpu->L),
        NULL,
        &(cpu->A),      
    };

    // Regpair mappings
    uint16_t *regpair_mapping[4] = {
        &(cpu->BC),
        &(cpu->DE),
        &(cpu->HL),
        &(cpu->SP),    
    };

    // Regpair names
    char *regpair_naming[4] = {
        "BC",
        "DE",
        "HL",
        "SP"
    };

    // This is DIFFERENT that SP is actually PSW as we can't push/pop SP
    // We're gonna leave PSW out for now and get back to it later since the
    // logic is slightly different
    uint16_t *regpair_pushpop_mapping[3] = {
        &(cpu->BC),
        &(cpu->DE),
        &(cpu->HL)  
    };
    char *regpair_pushpop_naming[3] = {
        "BC",
        "DE",
        "HL"
    };

    // Temp 8-bit data
    uint8_t src_byte = 0;
    uint8_t dst_byte = 0;

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
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->A == 0xFF) {
                cpu->A = 0x00;
	    	cpu->FLAGS.C = 0x01;
	        cpu->FLAGS.Z = 0x01;
	    } else
                cpu->A += 1;
            cpu->PC += 1;
            break;

        // 0x04 - INC B
        case 0x04:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->B == 0xFF) {
                cpu->B = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
    	    } else
                cpu->B += 1;
            cpu->PC += 1;
            break;

        // 0x0C - INC C 
        case 0x0C:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->C == 0xFF) {
                cpu->C = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
	    } else
                cpu->C += 1;
            cpu->PC += 1;
            break;

        // 0x14 - INC D 
        case 0x14:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->D == 0xFF) {
                cpu->D = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
	    } else
                cpu->D += 1;
            cpu->PC += 1;
            break;

        // 0x1C - INC E 
        case 0x1C:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
           if(cpu->E == 0xFF) {
                cpu->E = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
	   } else
                cpu->E += 1;
            cpu->PC += 1;
            break;

        // 0x24 - INC H
        case 0x24:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->H == 0xFF) {
                cpu->H = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
	    } else
                cpu->H += 1;
            cpu->PC += 1;
            break;
            
        // 0x2C - INC L
        case 0x2C:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->L == 0xFF) {
                cpu->L = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
	    } else
                cpu->L += 1;
            cpu->PC += 1;
            break;
            
        // 0x34 - INC (HL)
        case 0x34:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->HL >= memory_size)
                PANIC("attempting to access memory out of bounds");
            if((cpu->HL - 2) < ROM_SIZE)
                PANIC("0x34 instruction will write into ROM");
            if(memory[cpu->HL] == 0xFF) {
                memory[cpu->HL] = 0x00;
		cpu->FLAGS.C = 0x01;
		cpu->FLAGS.Z = 0x01;
	    } else
                memory[cpu->HL] += 1;
            cpu->PC += 1;
            break;

        /*
        * DEC byte
        */

        // 0x3D - DEC A 
        case 0x3D:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->A == 0x00) {
                cpu->A = 0xFF;
	    	cpu->FLAGS.C = 0x01;
	    } else
                cpu->A -= 1;

	    if(cpu->A == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

        // 0x05 - DEC B
        case 0x05:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->B == 0x00) {
                cpu->B = 0xFF;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->B -= 1;
	    if(cpu->B == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

        // 0x0D - DEC C
        case 0x0D:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->C == 0x00) {
                cpu->C = 0xFF;
		cpu->FLAGS.C = 0x01;
    	    } else
                cpu->C -= 1;
	    if(cpu->C == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

        // 0x15 - DEC D
        case 0x15:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->D == 0x00) {
                cpu->D = 0xFF;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->D -= 1;
	    if(cpu->D == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

        // 0x1D - DEC E 
        case 0x1D:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
           if(cpu->E == 0x00) {
                cpu->E = 0xFF;
		cpu->FLAGS.C = 0x01;
	   } else
                cpu->E -= 1;
	    if(cpu->E == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

        // 0x25 - DEC H
        case 0x25:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
           if(cpu->H == 0x00) {
                cpu->H = 0xFF;
		cpu->FLAGS.C = 0x01;
	   } else
                cpu->H -= 1;
	    if(cpu->H == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;
            
        // 0x2D - DEC L
        case 0x2D:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
           if(cpu->L == 0x00) {
                cpu->L = 0xFF;
		cpu->FLAGS.C = 0x01;
	   } else
                cpu->L -= 1;
	    if(cpu->L == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;
            
        // 0x34 - DEC (HL)
        case 0x35:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->HL >= memory_size)
                PANIC("attempting to access memory out of bounds");
            if((cpu->HL - 2) < ROM_SIZE)
                PANIC("0x35 instruction will write into ROM");
            if(memory[cpu->HL] == 0x00) {
                memory[cpu->HL] = 0xFF;
		cpu->FLAGS.C = 0x01;
	    } else
                memory[cpu->HL] -= 1;
	    if(memory[cpu->HL] == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

        /*
        * INC register pair
        */

       // 0x03 - INC BC
       case 0x03:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->BC == 0xFFFF) {
                cpu->BC = 0x0000;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->BC += 1;
	    if(cpu->BC == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

       // 0x13 - INC DE        
        case 0x13:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->DE == 0xFFFF) {
                cpu->DE = 0x0000;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->DE += 1;
	    if(cpu->DE == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

       // 0x23 - INC HL        
        case 0x23:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->HL == 0xFFFF) {
                cpu->HL = 0x0000;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->HL += 1;
	    if(cpu->HL == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

       // 0x33 - INC SP        
        case 0x33:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->SP == 0xFFFF) {
                cpu->SP = 0x0000;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->SP += 1;
            cpu->PC += 1;
	    if(cpu->SP == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            break;

        /*
        * DEC register pair
        */

       // 0x0B - DEC BC
       case 0x0B:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->BC == 0x0000) {
                cpu->BC = 0xFFFF;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->BC -= 1;
	    if(cpu->BC == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

       // 0x1B - DEC DE
       case 0x1B:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->DE == 0x0000) {
                cpu->DE = 0xFFFF;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->DE -= 1;
	    if(cpu->DE == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

       // 0x2B - DEC HL
       case 0x2B:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->HL == 0x0000) {
                cpu->HL = 0xFFFF;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->HL -= 1;
	    if(cpu->HL == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
            break;

       // 0x3B - DEC SP
       case 0x3B:
            cpu->FLAGS.C = 0x00;
	    cpu->FLAGS.Z = 0x00;
            if(cpu->SP == 0x0000) {
                cpu->SP = 0xFFFF;
		cpu->FLAGS.C = 0x01;
	    } else
                cpu->SP -= 1;
	    if(cpu->SP == 0x00) {
	    	cpu->FLAGS.Z = 0x01;
	    }
            cpu->PC += 1;
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
        
        // 0xC2 = JNZ 0x0000 
        case 0xC2:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("C2 instruction jumped outside memory bounds");
            if(cpu->FLAGS.Z == 0)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;

        // 0xCA = JZ 0x0000 
        case 0xCA:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("CA instruction jumped outside memory bounds");
            if(cpu->FLAGS.Z == 1)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;

        // 0xD2 = JNC 0x0000 
        case 0xD2:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("D2 instruction jumped outside memory bounds");
            if(cpu->FLAGS.C == 0)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;

        // 0xDA = JC 0x0000 
        case 0xDA:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("DA instruction jumped outside memory bounds");
            if(cpu->FLAGS.C == 1)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;

/*
 * Since we don't set parity even/odd in our emulator or sign bits, ignore these instructions
        // 0xE2 = JPO 0x0000 
        case 0xE2:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("E2 instruction jumped outside memory bounds");
            if(cpu->FLAGS.P == 0)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;

        // 0xEA = JPE 0x0000 
        case 0xEA:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("EA instruction jumped outside memory bounds");
            if(cpu->FLAGS.P == 1)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;

        // 0xF2 = JP 0x0000 
        case 0xF2:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("F2 instruction jumped outside memory bounds");
            if(cpu->FLAGS.S == 0)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;


        // 0xFA = JM 0x0000 
        case 0xFA:
            GET_WORD(tmp);
            if(tmp >= memory_size)
                PANIC("FA instruction jumped outside memory bounds");
            if(cpu->FLAGS.S == 1)
                cpu->PC = tmp;
            else
                cpu->PC += 3;
            break;
*/

        // 0xC9 = RET
        case 0xC9:
            // Prevent stack underflow
            if(cpu->SP > STACK_START-2)
                PANIC("C9 instruction will underflow stack");
            
            // Pop address off stack in correct memory order. 
            cpu->PC = memory[cpu->SP + 1] | (memory[cpu->SP] << 8);
            cpu->SP = cpu->SP + 2;
            break;

        // 0x36 = MVI (HL), BYTE
        case 0x36:
            GET_BYTE(tmp);
            if(cpu->HL >= memory_size)
                PANIC("attempting to access memory out of bounds");
            if((cpu->HL - 2) < ROM_SIZE)
                PANIC("0x36 instruction will write into ROM");
            memory[cpu->HL] = tmp;
            cpu->PC = cpu->PC + 2;
            break;

        // MOV register to register instructions
        // MOV B, <REG> Instructions
        case 0x40: 
        case 0x41: 
        case 0x42: 
        case 0x43: 
        case 0x44: 
        case 0x45: 
        case 0x47: 

        // MOV C, <REG> Instructions
        case 0x48: 
        case 0x49: 
        case 0x4a: 
        case 0x4b: 
        case 0x4c: 
        case 0x4d: 
        case 0x4f: 

        // MOV D, <REG> Instructions
        case 0x50: 
        case 0x51: 
        case 0x52: 
        case 0x53: 
        case 0x54: 
        case 0x55: 
        case 0x57: 

        // MOV E, <REG> Instructions
        case 0x58: 
        case 0x59: 
        case 0x5a: 
        case 0x5b: 
        case 0x5c: 
        case 0x5d: 
        case 0x5f: 

        // MOV H, <REG> Instructions
        case 0x60: 
        case 0x61: 
        case 0x62: 
        case 0x63: 
        case 0x64: 
        case 0x65: 
        case 0x67: 

        // MOV L, <REG> Instructions
        case 0x68: 
        case 0x69: 
        case 0x6a: 
        case 0x6b: 
        case 0x6c: 
        case 0x6d: 
        case 0x6f: 

        // MOV A, <REG> Instructions
        case 0x78: 
        case 0x79: 
        case 0x7a: 
        case 0x7b: 
        case 0x7c: 
        case 0x7d: 
        case 0x7f: 
            src_byte = 0b00000111 & memory[cpu->PC];
            dst_byte = 0b00000111 & (memory[cpu->PC] >> 3);
            (*reg_mapping[dst_byte]) = (*reg_mapping[src_byte]);
            cpu->PC = cpu->PC + 1;
            break;

        // CPI instructions
        case 0xFE:
            GET_BYTE(tmp8);
            int16_t res = cpu->A - tmp8;
            cpu->FLAGS.C = (res >> 8) & 0x1;
            cpu->FLAGS.Z = (res == 0x0);
            cpu->PC = cpu->PC + 2;
            break;

        // PUSH B
        case 0xC5:
            if(cpu->SP < ROM_SIZE + 2)
                PANIC("0xC5 instruction will write into ROM");
            memory[cpu->SP - 2] = cpu->C;
            memory[cpu->SP - 1] = cpu->B;
            cpu->SP = cpu->SP - 2;
            cpu->PC = cpu->PC + 1;
            break;

        // PUSH D
        case 0xD5:
            if(cpu->SP < ROM_SIZE + 2)
                PANIC("0xD5 instruction will write into ROM");
            memory[cpu->SP - 2] = cpu->E;
            memory[cpu->SP - 1] = cpu->D;
            cpu->SP = cpu->SP - 2;
            cpu->PC = cpu->PC + 1;
            break;
        
        // PUSH H
        case 0xE5:
            if(cpu->SP < ROM_SIZE + 2)
                PANIC("0xE5 instruction will write into ROM");
            memory[cpu->SP - 2] = cpu->L;
            memory[cpu->SP - 1] = cpu->H;
            cpu->SP = cpu->SP - 2;
            cpu->PC = cpu->PC + 1;
            break;

        // DAD Instructions
        case 0x09:
        case 0x19: 
        case 0x29:
        case 0x39:
            src_byte = 0b00000011 & (memory[cpu->PC] >> 4);
            tmp32 = cpu->HL + (*regpair_mapping[src_byte]);
            cpu->HL = 0xFFFF & tmp32;
            cpu->FLAGS.C = (tmp32 >> 16) & 0x1;
            cpu->PC = cpu->PC + 1;
            break;

        // XCHG instruction
        case 0xEB: 
            tmp = cpu->HL;
            cpu->HL = cpu->DE;
            cpu->DE = tmp;
            cpu->PC = cpu->PC + 1;
            break;

        // POP instructions
        case 0xC1:
        case 0xD1: 
        case 0xE1: 
            if(cpu->SP > STACK_START-2)
                PANIC("%02X instruction will underflow stack", memory[cpu->PC]);
            src_byte = 0b00000011 & (memory[cpu->PC] >> 4);
            (*regpair_pushpop_mapping[src_byte]) =
                (memory[cpu->SP] << 8) | memory[cpu->SP+1];
            cpu->SP = cpu->SP + 2;
            cpu->PC = cpu->PC + 1;
            break;

        // OUT instruction
        // We're just going to pretend to write out and log the event
        case 0xD3:
            printf("> OUT (%02X) -> %02X (A)\n", memory[cpu->PC], cpu->A);
            cpu->PC = cpu->PC + 2;
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
