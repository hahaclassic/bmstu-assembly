#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#define QWORD 8
#define QQWORD 32
#define MAX_QLTY 100
#define _JPG 3
#define _PNG 4

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("[ERR]: invalid args. Example: ./main.exe in.img out.img [added brightness level]\n");
        return EXIT_FAILURE;
    }
    int brightness = atoi(argv[3]);
    unsigned char *qqword_ptr = malloc(QQWORD);
    if (brightness < 0) 
        memset(qqword_ptr, -brightness, QQWORD);
    else 
        memset(qqword_ptr, brightness, QQWORD);

    int width, height, n;
    unsigned char *image = stbi_load(argv[1], &width, &height, &n, _JPG);
    unsigned char *curr_byte_vector = image;
  
    int num_of_vec = width * height * n / QQWORD;
    for (int i = 0; i < num_of_vec; i++)
    {
        if (brightness > 0) {
            __asm__ (
                "vmovups ymm0, [%1]\n\t"
                "vmovups ymm1, [%2]\n\t"
                "vpaddusb ymm2, ymm0, ymm1\n\t"
                "vmovups [%0], ymm2\n\t"
                : "=r" (curr_byte_vector)
                : "r" (curr_byte_vector), "r" (qqword_ptr)
                : "ymm0", "ymm1", "ymm2"
            );
        } else {
            __asm__ (
                "vmovups ymm0, [%1]\n\t"
                "vmovups ymm1, [%2]\n\t"
                "vpsubusb ymm2, ymm0, ymm1\n\t"
                "vmovups [%0], ymm2\n\t"
                : "=r" (curr_byte_vector)
                : "r" (curr_byte_vector), "r" (qqword_ptr)
                : "ymm0", "ymm1", "ymm2"
            );
        }

        curr_byte_vector += QQWORD;
    }
    
    stbi_write_jpg(argv[2], width, height, n, image, MAX_QLTY);
    stbi_image_free(image);
    free(qqword_ptr);
    printf("Done.\n");

    return EXIT_SUCCESS;
}
