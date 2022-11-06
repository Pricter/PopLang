#include <stdio.h>
#include <stdint.h>
#include "utils.h"

int main(int argc, char** argv) {
    const char* program = shift(&argc, &argv);
    const char* input_file_path = NULL;
    if(argc < 1) {
        fprintf(stderr, "[ERROR] %s: No Input file provided.\n", program);
        exit(1);
    } else {
        input_file_path = shift(&argc, &argv);
        (void) argv;
    }
    FILE* f; fopen_s(&f, input_file_path, "rb");

    uint8_t* fileType = parseB(4, &f);
    if(!compareBytes(fileType, "popb", 4)) {
        printf("[ERROR] %s: File passed did not start with `popb`, instead it started with `", program);
        printBytes(fileType, 4, 0); printf("`\n");
    }
    printf("[INFO] %s: Parsing file `%s`\n", program, input_file_path);

    fclose(f);
    return 0; 
}