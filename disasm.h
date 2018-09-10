#ifndef DISASM_H
#define DIsASM_h

void OpToText(unsigned char* buffer, int* counter);
void Disassemble(unsigned char* buffer, int buffer_size);

#endif
