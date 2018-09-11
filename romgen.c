#include <stdio.h>
#include <stdlib.h>

#include "disasm.h"

// This program uses 
int main(int argc, char** argv)
{
    // Open the rom file
    FILE* f = fopen(argv[1], "rb");
    if(f == NULL) {
        perror("could not open file");
        exit(1);
    }
    
    // Grab the file size
    fseek(f, 0, SEEK_END);
    int sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read into buffer
    unsigned char* buf = malloc(sz);
    fread(buf, sz, 1, f);
    fclose(f);

    // Prepare the output file
    char filename[256];
    sprintf(filename, "%s.h", argv[1]);
    
    // write into the new rom file
    FILE *fw = fopen(filename, "w");
    if(fw == NULL) {
        perror("could not open file");
        exit(1);
    }

    // Start writing into the file
    fprintf(fw, "#ifndef ROMGEN_H\n#define ROMGEN_H\nstatic char rom[%X] = {\n", sz);

    // Convert to hex
    unsigned int i = 0;
    for(i = 0; i < sz; i++) {
        if(i % 8 == 0)
            fprintf(fw, "\n");
        if(i % 8 == 0)
            fprintf(fw, "/* 0x%04X */ ", i);
        fprintf(fw, "0x%02X", buf[i]);
        if(i < sz-1)
            fprintf(fw,", ");
    } 

    char* footer_char = "\n};\n#endif";
    fprintf(fw, "\n};\n#endif");
    fclose(fw);

    free(buf);
    return 0;
}
