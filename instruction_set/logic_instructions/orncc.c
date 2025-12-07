#include "instruction_set.h"
#include <malloc.h>

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
  orncc_data_t* data = malloc(sizeof(orncc_data_t));
  
  data->src1 = src1;
  data->src2 = src2;
  data->dest = dest;

  return create_instruction(orncc_execute, data);
}
