#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "config.h"
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
    if(sz > MEMORY_SIZE) {
        printf("Error. ROM of size 0x%X overflows MEMORY_SIZE of size 0x%X", sz, MEMORY_SIZE);
        exit(1);
    }

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
    fprintf(fw, "#ifndef ROMGEN_H\n#define ROMGEN_H\n#define ROM_SIZE 0x%X\nstatic uint8_t prom[MEMORY_SIZE] = {\n", sz);

    // Convert to hex
    unsigned int i = 0;
    for(i = 0; i < sz; i++) {
        // Line break and show offset
        if(i % 8 == 0) {
            fprintf(fw, "\n");
            fprintf(fw, "/* 0x%04X */ ", i);
        }
        // Write bytes to file, or pad with 0s if we're out of things to write
        if(i < sz)
            fprintf(fw, "0x%02X", buf[i]);
        else
            fprintf(fw, "0x00");

        // Don't add a comma at the end
        if(i < MEMORY_SIZE-1)
            fprintf(fw,", ");
    } 

    // Cleanup 
    fprintf(fw, "\n};\n#endif");
    fclose(fw);
    free(buf);

    return 0;
}
