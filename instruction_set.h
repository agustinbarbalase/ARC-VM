#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <stdbool.h>

typedef void (*execute_fn_t)(int* registers, bool* running, void* data);

typedef struct instruction instruction_t;

void execute_instruction(instruction_t* instr, int* registers, bool* running);

instruction_t* addcc(int src1, int src2, int dest);

instruction_t* srl(int src1, int src2, int dest);

instruction_t* andcc(int src1, int src2, int dest);

instruction_t* orcc(int src1, int src2, int dest);

instruction_t* orncc(int src1, int src2, int dest);

instruction_t* nop();

#endif