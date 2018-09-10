#ifndef DISASM_H
#define DIsASM_h

int op_to_text(unsigned char* buffer, int* counter);
int disassemble(unsigned char* buffer, int buffer_size);

#endif
