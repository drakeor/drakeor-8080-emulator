#ifndef DISASM_H
#define DIsASM_h

void OpToText(unsigned char* buffer, int* counter, char* msg, char msg_len);
void Disassemble(unsigned char* buffer, int buffer_size);

#endif
