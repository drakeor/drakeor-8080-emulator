#include <stdio.h>
#include "vram.h"


int vram_to_bmp() {
    
    // Validate the VRAM configuration.
    if(8 * (MEMORY_SIZE - VRAM_START) < VRAM_DIM_X * VRAM_DIM_Y) {
        return -1;
    }

    // VRAM_DIM_X needs to be a factor of 8
    if(VRAM_DIM_X % 8 != 0) {
        return -1;
    }

    /*
    * Body of this function is taken from https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
    * Modified to use existing VRAM instead of allocating a new image on the heap 
    */
    FILE *f;
    int filesize = 54 + 3*VRAM_DIM_X*VRAM_DIM_Y;

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       VRAM_DIM_X    );
    bmpinfoheader[ 5] = (unsigned char)(       VRAM_DIM_X>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       VRAM_DIM_X>>16);
    bmpinfoheader[ 7] = (unsigned char)(       VRAM_DIM_X>>24);
    bmpinfoheader[ 8] = (unsigned char)(       VRAM_DIM_Y    );
    bmpinfoheader[ 9] = (unsigned char)(       VRAM_DIM_Y>> 8);
    bmpinfoheader[10] = (unsigned char)(       VRAM_DIM_Y>>16);
    bmpinfoheader[11] = (unsigned char)(       VRAM_DIM_Y>>24);

    f = fopen("vram.bmp","wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    /* Remember that our vram is stored 1 bit per pixel
     * Since BMPs are 24bits per pixel, we need to do some conversions before writing.
     */
    int vram_dim_x_bytes = (VRAM_DIM_X / 8);
    for(int y=0; y < VRAM_DIM_Y; y++) {
        for(int x=0; x < vram_dim_x_bytes; x++) {

            // Grab the first 8 bits of the row
            unsigned char PIXEL = prom[VRAM_START + x + (y * vram_dim_x_bytes)];

            // Loop through each bit and write to the BMP file
            for(int i=0; i<8; i++) {
                if(((PIXEL << i) & 0x80) == 0x80) {
                    unsigned char pixel[] = { 0xFF, 0xFF, 0xFF };
                    fwrite(pixel,sizeof(unsigned char),3,f);
                } else {
                    unsigned char pixel[] = { 0x00, 0x00, 0x00 };
                    fwrite(pixel,sizeof(unsigned char),3,f);
                }
            }
        }
    }
    fclose(f);

    return 0;
}
