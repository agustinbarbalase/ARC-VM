#include "instruction_set.h"

#include <malloc.h>

typedef struct instruction {
  execute_fn_t execute;
  void* data;
} instruction_t;

void execute_instruction(instruction_t* instr, int* registers, bool* running) {
  instr->execute(registers, running, instr->data);
  if (!instr->data) {
    free(instr->data);
  }
  free(instr);
}

typedef struct addcc_data {
  operand_t src1;
  operand_t src2;
  operand_t dest;
} addcc_data_t;

static void addcc_execute(int* regs, bool* running, void* data) {
  (void)running;
  addcc_data_t* d = data;
  int val1 = (d->src1.type == REGISTER) ? regs[d->src1.value] : d->src1.value;
  int val2 = (d->src2.type == REGISTER) ? regs[d->src2.value] : d->src2.value;
  int dest = (d->dest.type == REGISTER) ? d->dest.value : -1;
  if (dest >= 0)
    regs[dest] = val1 + val2;
}

instruction_t* addcc(operand_t src1, operand_t src2, operand_t dest) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  addcc_data_t* d = malloc(sizeof(addcc_data_t));

  d->src1 = src1;
  d->src2 = src2;
  d->dest = dest;

  inst->execute = addcc_execute;
  inst->data = d;

  return inst;
}

typedef struct srl_data {
  operand_t src1;
  operand_t src2;
  operand_t dest;
} srl_data_t;

static void srl_execute(int* regs, bool* running, void* data) {
  (void)running;
  srl_data_t* d = data;
  int val1 = (d->src1.type == REGISTER) ? regs[d->src1.value] : d->src1.value;
  int val2 = (d->src2.type == REGISTER) ? regs[d->src2.value] : d->src2.value;
  int dest = (d->dest.type == REGISTER) ? d->dest.value : -1;
  if (dest >= 0)
    regs[dest] = (unsigned int)val2 >> val1;
}

instruction_t* srl(operand_t src1, operand_t src2, operand_t dest) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  srl_data_t* d = malloc(sizeof(srl_data_t));

  d->src1 = src1;
  d->src2 = src2;
  d->dest = dest;

  inst->execute = srl_execute;
  inst->data = d;

  return inst;
}

typedef struct andcc_data {
  operand_t src1;
  operand_t src2;
  operand_t dest;
} andcc_data_t;

static void andcc_execute(int* regs, bool* running, void* data) {
  (void)running;
  andcc_data_t* d = data;
  int val1 = (d->src1.type == REGISTER) ? regs[d->src1.value] : d->src1.value;
  int val2 = (d->src2.type == REGISTER) ? regs[d->src2.value] : d->src2.value;
  int dest = (d->dest.type == REGISTER) ? d->dest.value : -1;
  if (dest >= 0)
    regs[dest] = val1 & val2;
}

instruction_t* andcc(operand_t src1, operand_t src2, operand_t dest) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  andcc_data_t* d = malloc(sizeof(andcc_data_t));

  d->src1 = src1;
  d->src2 = src2;
  d->dest = dest;

  inst->execute = andcc_execute;
  inst->data = d;

  return inst;
}

typedef struct orcc_data {
  operand_t src1;
  operand_t src2;
  operand_t dest;
} orcc_data_t;

static void orcc_execute(int* regs, bool* running, void* data) {
  (void)running;
  orcc_data_t* d = data;
  int val1 = (d->src1.type == REGISTER) ? regs[d->src1.value] : d->src1.value;
  int val2 = (d->src2.type == REGISTER) ? regs[d->src2.value] : d->src2.value;
  int dest = (d->dest.type == REGISTER) ? d->dest.value : -1;
  if (dest >= 0)
    regs[dest] = val1 | val2;
}

instruction_t* orcc(operand_t src1, operand_t src2, operand_t dest) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  orcc_data_t* d = malloc(sizeof(orcc_data_t));

  d->src1 = src1;
  d->src2 = src2;
  d->dest = dest;

  inst->execute = orcc_execute;
  inst->data = d;

  return inst;
}

typedef struct orncc_data {
  operand_t src1;
  operand_t src2;
  operand_t dest;
} orncc_data_t;

static void orncc_execute(int* regs, bool* running, void* data) {
  (void)running;
  orncc_data_t* d = data;
  int val1 = (d->src1.type == REGISTER) ? regs[d->src1.value] : d->src1.value;
  int val2 = (d->src2.type == REGISTER) ? regs[d->src2.value] : d->src2.value;
  int dest = (d->dest.type == REGISTER) ? d->dest.value : -1;
  if (dest >= 0)
    regs[dest] = ~(val1 | val2);
}

instruction_t* orncc(operand_t src1, operand_t src2, operand_t dest) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  orncc_data_t* d = malloc(sizeof(orncc_data_t));

  d->src1 = src1;
  d->src2 = src2;
  d->dest = dest;

  inst->execute = orncc_execute;
  inst->data = d;

  return inst;
}

static void nop_execute(int* regs, bool* running, void* data) {
  (void)regs;
  (void)data;
  *running = false;
}

instruction_t* nop() {
  instruction_t* inst = malloc(sizeof(instruction_t));

  inst->execute = nop_execute;
  inst->data = NULL;

  return inst;
}
