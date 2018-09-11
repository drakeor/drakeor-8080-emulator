#ifndef DISASM_H
#define DIsASM_h

// Prints the opcode to text
// Note this will automatically advance the counter
int op_to_text(unsigned char* buffer, int buffer_size, int* counter);

// Attempts to disassemble a ROM.
// Not guaranteed to work 100% given that some roms have
// data interleaved
int disassemble(unsigned char* buffer, int buffer_size);

#endif
