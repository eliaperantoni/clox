#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int rgc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    for (int i = 0; i < 500; i++) {
        writeConstant(&chunk, i, 1);
    }

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}
