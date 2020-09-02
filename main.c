#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int rgc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    writeChunk(&chunk, OP_RETURN, 1);
    writeChunk(&chunk, OP_RETURN, 2);
    writeChunk(&chunk, OP_RETURN, 2);
    writeChunk(&chunk, OP_RETURN, 3);
    writeChunk(&chunk, OP_RETURN, 3);
    writeChunk(&chunk, OP_RETURN, 3);
    writeChunk(&chunk, OP_RETURN, 4);
    writeChunk(&chunk, OP_RETURN, 4);
    writeChunk(&chunk, OP_RETURN, 4);
    writeChunk(&chunk, OP_RETURN, 4);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}
