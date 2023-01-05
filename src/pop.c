#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include <time.h>
#define _DEBUG

int __temp_prog_nc = 0;

#define MAX_STACK_SIZE 1024 * 1024

typedef struct stackI {
    int64_t *stack;
    size_t stackPtr;
    int stackSize;
} stackI;

typedef struct progI {
    int opCount;
    int pSize;
    uint8_t *prog;
} progI;

enum {
    OP_PUSH = 1,
    OP_POP = 2,
};

void execute_prog(stackI *stack, progI *program) {
    char __temp_pname[16];
    sprintf(__temp_pname, "prog%d.popt", __temp_prog_nc);
    __temp_prog_nc++;
    FILE *pf = writeBytes(program->prog, program->pSize, __temp_pname);
    for(int i = 0; i < program->opCount; i++) {
        int64_t opSize = parse2(&pf);
        int64_t op = parse2(&pf);
        printf("OP_Size: %ld, ", opSize);
        switch (op) {
            case OP_PUSH:
                int negative = bytesToInt(parseB(1, &pf), 1); (void)negative;
                int64_t to_push = parse4(&pf);
#ifdef _DEBUG
                printf("OP: PUSH(%ld)\n", to_push);
#endif
                if(negative) to_push = -to_push;
                stack->stack[stack->stackPtr++] = to_push;
                stack->stackSize++;
        }
    }
    (void)stack;
    return;
}

void execute_file(FILE** f, stackI *stack) {
    progI progInfo = {0};
    progInfo.opCount = parse4(f);
    progInfo.pSize = parse4(f);
    progInfo.prog = parseB(progInfo.pSize, f);
    printf("\n\nOP Count: %d\n", (int)progInfo.opCount);
    printf("Code Size: %d\n", (int)progInfo.pSize);
    printf("Starting execution...\n");
    printf("-------------------------------\n");

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    execute_prog(stack, &progInfo);
    clock_gettime(CLOCK_MONOTONIC, &end);

    printStack(stack->stack, stack->stackSize);

    long elapsed_ns = (end.tv_sec - start.tv_sec) * 1e9 +
        (end.tv_nsec - start.tv_nsec);
    long elapsed_us = elapsed_ns / 1e3;
    printf("[INFO] Executed in %ldus\n", elapsed_us);
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
    FILE* f = fopen(input_file_path, "rb");
    if(f == NULL) {
        fprintf(stderr, "[ERROR] %s: %s does not exist.\n", program, input_file_path);
        exit(1);
    }

    uint8_t *fileType = parseB(4, &f);
    if(!compareBytes(fileType, "POPB", 4)) {
        fprintf(stderr, "[ERROR] %s: File passed did not start with `popb`, instead it started with `", program);
        printBytes(fileType, 4, 0); fprintf(stderr, "`\n");
    }
    printf("[INFO] %s: Parsing file `%s`\n", program, input_file_path);

    stackI stack = {0};
    stack.stack = malloc(sizeof(int64_t) * MAX_STACK_SIZE);
    execute_file(&f, &stack);

    fclose(f); 

    return 0;
}