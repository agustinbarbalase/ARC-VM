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
  int src1;
  int src2;
  int dest;
} addcc_data_t;

static void addcc_execute(int* regs, bool* running, void* data) {
  addcc_data_t* d = data;
  regs[d->dest] = d->src1 + d->src2;
}

instruction_t* addcc(int src1, int src2, int dest) {
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
  int src1;
  int src2;
  int dest;
} srl_data_t;

static void srl_execute(int* regs, bool* running, void* data) {
  srl_data_t* d = data;
  regs[d->dest] = (unsigned int)d->src2 >> d->src1;
}

instruction_t* srl(int src1, int src2, int dest) {
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
  int src1;
  int src2;
  int dest;
} andcc_data_t;

static void andcc_execute(int* regs, bool* running, void* data) {
  andcc_data_t* d = data;
  regs[d->dest] = d->src1 & d->src2;
}

instruction_t* andcc(int src1, int src2, int dest) {
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
  int src1;
  int src2;
  int dest;
} orcc_data_t;

static void orcc_execute(int* regs, bool* running, void* data) {
  orcc_data_t* d = data;
  regs[d->dest] = d->src1 | d->src2;
}

instruction_t* orcc(int src1, int src2, int dest) {
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
  int src1;
  int src2;
  int dest;
} orncc_data_t;

static void orncc_execute(int* regs, bool* running, void* data) {
  orncc_data_t* d = data;
  regs[d->dest] = ~(d->src1 | d->src2);
}

instruction_t* orncc(int src1, int src2, int dest) {
  instruction_t* inst = malloc(sizeof(instruction_t));
  orncc_data_t* d = malloc(sizeof(orncc_data_t));

  d->src1 = src1;
  d->src2 = src2;
  d->dest = dest;

  inst->execute = orncc_execute;
  inst->data = d;

  return inst;
}

static void nop_execute(int* regs, bool* running, void* data) { *running = false; }

instruction_t* nop() {
  instruction_t* inst = malloc(sizeof(instruction_t));

  inst->execute = nop_execute;
  inst->data = NULL;

  return inst;
}
