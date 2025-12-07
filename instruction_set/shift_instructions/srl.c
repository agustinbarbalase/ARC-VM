#include "instruction_set.h"
#include <malloc.h>

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
  srl_data_t* data = malloc(sizeof(srl_data_t));

  data->src1 = src1;
  data->src2 = src2;
  data->dest = dest;

  return create_instruction(srl_execute, data);
}
