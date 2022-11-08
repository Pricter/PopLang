#include <stdio.h>
#include <stdint.h>
#include "utils.h"

#define OP_PUSH 1

#define parse2(f) bytesToInt(parseB(2, &pf), 2)
#define parse4(f) bytesToInt(parseB(4, &pf), 4)

void execute(char *program, FILE** f) {
    int stop = 0;
    uint8_t *opCount = parseB(4, f);
    uint8_t *toRead = parseB(4, f);
    int64_t toReadN = bytesToInt(toRead, 4);
    uint8_t *progB = parseB(toReadN, f);
    FILE *pf = writeBytes(progB, toReadN, "..\\bin\\test.ppopd");
    for(int i = 0; i < bytesToInt(opCount, 4); i++) {
        int64_t opSize = parse2(&pf);
        int64_t op = parse2(&pf);
        if(op == OP_PUSH) {
            int64_t pushNum = parse4(&pf);
            printf("OP %i, size: %lld, OP Type: (PUSH %lld)\n", i, opSize, pushNum);
        }
    }
}

int main(int argc, char** argv) {
    const char *program = shift(&argc, &argv);
    const char *input_file_path = NULL;
    if(argc < 1) {
        fprintf(stderr, "[ERROR] %s: No Input file provided.\n", program);
        exit(1);
    } else {
        input_file_path = shift(&argc, &argv);
        (void) argv;
    }
    FILE* f; fopen_s(&f, input_file_path, "rb");

    uint8_t *fileType = parseB(4, &f);
    if(!compareBytes(fileType, "popb", 4)) {
        fprintf(stderr, "[ERROR] %s: File passed did not start with `popb`, instead it started with `", program);
        printBytes(fileType, 4, 0); fprintf(stderr, "`\n");
    }
    printf("[INFO] %s: Parsing file `%s`\n", program, input_file_path);

    execute(program, &f);

    fclose(f);
    return 0; 
}