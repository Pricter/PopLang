#include <stdio.h>
#include <stdint.h>
#include "utils.h"

#define MAX_STACK_SIZE 1024 * 1024  
int64_t *stack;
size_t stackPtr = 0;
uint64_t stackSize;

enum {
    OP_PUSH = 1,
    OP_POP = 2,
    OP_DUMP = 3,
    OP_ADD = 4,
    OP_SUB = 5,
};

#define parse2(f) bytesToInt(parseB(2, f), 2)
#define parse4(f) bytesToInt(parseB(4, f), 4)

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
            stack[stackPtr] = (int64_t)pushNum;
            stackPtr++; stackSize++;
            printf("OP %i, size: %lld, OP Type: (PUSH %lld), stack size %lld, stack pointer %lld\n", i, opSize, pushNum, stackSize,
                    (uint64_t)stackPtr);
            printStack(stack, stackSize);
        } else if(op == OP_POP) {
            int64_t popNum = parse2(&pf);
            if(popNum > stackSize) {
                fprintf(stderr, "[ERROR] Cannot pop %i elements from stack. stack has %i elements", popNum, stackSize);
                exit(1);
            }
            for(int i = 1; i <= popNum; i++) {
                stack[stackSize - i] = 0;
                stackPtr--; stackSize--;
            }
            printf("OP %i, size: %lld, OP Type: (POP %lld), stack size %lld, stack pointer %lld\n", i, opSize, popNum, stackSize,
                    (uint64_t)stackPtr);
            printStack(stack, stackSize);
        } else if(op == OP_DUMP) {
            printf("OP %i, size: %lld, OP Type: (DUMP %i), stack size %lld, stack pointer %lld\n", i, opSize, stack[stackPtr - 1], 
                    stackSize, (uint64_t)stackPtr);
            printStack(stack, stackSize);
        } else if(op == OP_ADD) {
            if(stackSize < 2) {
                fprintf(stderr, "[ERROR] Not enough stack members to perform ADD.");
                printStack(stack, stackSize);
                exit(1);
            }
            int64_t a = stack[stackPtr - 1]; stack[stackPtr - 1] = 0;
            int64_t b = stack[stackPtr - 2]; stack[stackPtr - 2] = 0;
            stackPtr--; stackSize--;
            stack[stackPtr - 1] = a + b;
            printf("OP %i, size: %lld, OP Type: (ADD %i + %i = %i), stack size %lld, stack pointer %lld\n", i, opSize, a, b, stack[stackPtr - 1],
                    stackSize, (uint64_t)stackPtr);
            printStack(stack, stackSize);
        } else if(op == OP_SUB) {
            if(stackSize < 2) {
                fprintf(stderr, "[ERROR] Not enough stack members to perform SUB.");
                printStack(stack, stackSize);
                exit(1);
            }
            int64_t a = stack[stackPtr - 1]; stack[stackPtr - 1] = 0;
            int64_t b = stack[stackPtr - 2]; stack[stackPtr - 2] = 0;
            stackPtr--; stackSize--;
            stack[stackPtr - 1] = b - a;
            printf("OP %i, size: %lld, OP Type: (SUB %i - %i = %i), stack size %lld, stack pointer %lld\n", i, opSize, a, b, stack[stackPtr - 1],
                    stackSize, (uint64_t)stackPtr);
            printStack(stack, stackSize);
        } else {
            fprintf(stderr, "[ERROR] Invalid opcode %x", op);
            exit(1);
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

    stack = malloc(MAX_STACK_SIZE);
    stackSize = 0;
    execute(program, &f);

    fclose(f);
    return 0; 
}