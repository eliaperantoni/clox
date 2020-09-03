#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"
#include "memory.h"

VM vm;

void initVM() {
    vm.count = 0;
    vm.capacity = 0;
    vm.stack = NULL;
}

void freeVM() {
    FREE_ARRAY(Value, vm.stack, vm.capacity);
    initVM();
}

void push(Value value) {
    if (vm.capacity < vm.count + 1) {
        int oldCapacity = vm.capacity;
        vm.capacity = GROW_CAPACITY(oldCapacity);
        vm.stack = GROW_ARRAY(Value, vm.stack,
                                 oldCapacity, vm.capacity);
    }

    vm.stack[vm.count] = value;
    vm.count++;
}

Value pop() {
    vm.count--;
    return vm.stack[vm.count];
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (int idx = 0; idx < vm.count; idx++) {
            printf("[ ");
            printValue(vm.stack[idx]);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
            case OP_NEGATE:   push(-pop()); break;
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}
