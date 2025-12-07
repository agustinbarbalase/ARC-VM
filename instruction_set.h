#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

#include <stdbool.h>

typedef void (*execute_fn_t)(int* registers, bool* running, void* data);

typedef struct instruction instruction_t;

typedef enum { IMMEDIATE, REGISTER } operand_type_t;

typedef struct operand {
  operand_type_t type;
  int value;
} operand_t;

void execute_instruction(instruction_t* instr, int* registers, bool* running);

instruction_t* addcc(operand_t src1, operand_t src2, operand_t dest);

instruction_t* srl(operand_t src1, operand_t src2, operand_t dest);

instruction_t* andcc(operand_t src1, operand_t src2, operand_t dest);

instruction_t* orcc(operand_t src1, operand_t src2, operand_t dest);

instruction_t* orncc(operand_t src1, operand_t src2, operand_t dest);

instruction_t* nop();

#endif