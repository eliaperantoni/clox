#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initValueArray(&chunk->constants);
    initNewlineArray(&chunk->newlines);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeValueArray(&chunk->constants);
    freeNewlineArray(&chunk->newlines);
    initChunk(chunk);
}

int prevLine = 1;

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
                                 oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    if (line > prevLine) {
        prevLine = line;
        writeNewlineArray(&chunk->newlines, chunk->count - 1);
    }
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void initNewlineArray(NewlineArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->newlines = NULL;
}

void writeNewlineArray(NewlineArray* array, int newline) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->newlines = GROW_ARRAY(int, array->newlines,
                                   oldCapacity, array->capacity);
    }

    array->newlines[array->count] = newline;
    array->count++;
}

void freeNewlineArray(NewlineArray* array) {
    FREE_ARRAY(int, array->newlines, array->capacity);
    initNewlineArray(array);
}
