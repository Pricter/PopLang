#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static char *shift(int *argc, char ***argv)
{
    assert(*argc > 0);
    char *result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}

uint8_t *convertToInt(uint8_t *bytes, int size) {
    uint8_t *intL = malloc(size);
    for(int i = 0; i < size; i++) {
        intL[i] = (int)bytes[i];
    }
    return intL;
}

uint8_t *parseB(int bytes, FILE** f) {
    uint8_t *buffer = malloc(bytes);
    fread(buffer, sizeof(buffer[0]), bytes, *f);
    return buffer;
}

void printBytes(uint8_t* bytes, int size, int likeHex) {
    if(likeHex) printf("0x");
    for(int i = 0; i < size; i++) {
        if(likeHex) {
            printf("%x", bytes[i]);
        } else {
            putc((char)bytes[i], stdout);
        }
    }
}

int compareBytes(uint8_t *bytes, char* s, int size) {
    for(int i = 0; i < size; i++) {
        if((int)bytes[i] != (int)s[i]) return 0;
    }
    return 1;
}

#endif