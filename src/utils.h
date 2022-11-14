#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

static char *shift(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

uint8_t *convertToInt(uint8_t *bytes, int size) {
    uint8_t *intL = malloc(size * sizeof(uint8_t));
    for(int i = 0; i < size; i++) {
        intL[i] = (int)bytes[i];
    }
    return intL;
}

void printBytes(uint8_t* bytes, int size, int likeHex) {
    if(likeHex) printf("0x");
    for(int i = 0; i < size; i++) {
        if(likeHex) {
            if(bytes[i] < 15) putc('0', stdout);
            printf("%x", bytes[i]);
        } else {
            putc((char)bytes[i], stdout);
        }
    }
}

uint8_t *parseB(uint64_t bytes, FILE** f) {
    uint8_t *buffer = malloc(bytes * sizeof(uint8_t));
    int ret = fread(buffer, sizeof(buffer[0]), bytes, *f);
    if(ret < bytes) {
        fprintf(stderr, "[ERROR] An error occured while %lld bytes, got %i bytes instead\n", bytes, ret);
        fprintf(stderr, "[ERROR] %s", strerror(errno));
        exit(1);
    }
    #ifdef LOG
    printf("[INFO] Read %i bytes, `", ret); printBytes(buffer, ret, 1); printf("`\n");
    #endif
    return buffer;
}

int compareBytes(uint8_t *bytes, char* s, int size) {
    for(int i = 0; i < size; i++) {
        if((int)bytes[i] != (int)s[i]) return 0;
    }
    return 1;
}

FILE *writeBytes(uint8_t* bytes, int64_t size, const char* path) {
    FILE* f; fopen_s(&f, path, "ab+");
    for(int i = 0; i < size; i++) {
        putc(bytes[i], f);
    }
    printf("[INFO] Wrote %i bytes to %s, `", size, path); printBytes(bytes, size, 1); printf("`\n");
    rewind(f);
    return f;
}

char *bytesToString(uint8_t *bytes, int size) {
    char *s = malloc(size * 2 + 1);
    char *curr_ptr = s;
    for(int i = 0; i < size; i++) {
        sprintf(curr_ptr, "%02x", bytes[i]);
        curr_ptr += 2;
    }
    s[size * 2 + 1] = '\0';
    return s;
}

int64_t bytesToInt(uint8_t* bytes, int size) {
    char *hex_s = bytesToString(bytes, size);
    int64_t result = (int64_t)strtol(hex_s, NULL, 16);
    return result;
}

void printStack(uint64_t *stack, uint64_t stackSize) {
    printf("Stack:\n");
    for(int i = 0; i < stackSize; i++) {
        printf("\t%i, i: %i\n", stack[i], i);
    }
}

#endif