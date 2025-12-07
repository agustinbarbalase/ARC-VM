#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct file {
  const char* filename;
  FILE* fp;
  size_t position;
} file_t;

file_t* init_file(const char* filename) {
  file_t* file = malloc(sizeof(file_t));
  file->filename = filename;
  file->fp = fopen(filename, "r");
  file->position = 0;
  return file;
}

bool parse_operand(const char* str, operand_t* out) {
  if (str[0] == '%') {
    int reg_num = -1;
    if (sscanf(str, "%%r%d", &reg_num) == 1 && reg_num >= 0) {
      out->type = REGISTER;
      out->value = reg_num;
      return true;
    }
    return false;
  } else {
    int val = 0;
    if (sscanf(str, "%d", &val) == 1) {
      out->type = IMMEDIATE;
      out->value = val;
      return true;
    }
    return false;
  }
}

instruction_t* fetch(file_t* file) {
  if (!file) {
    return NULL;
  }

  char line[128];
  if (fgets(line, sizeof(line), file->fp) == NULL) {
    return NULL;
  }

  char instr_name[16];
  char src1_str[16], src2_str[16], dest_str[16];

  if (sscanf(line, "%15s %15[^,], %15[^,], %15s", instr_name, src1_str, src2_str, dest_str) != 4) {
    return NULL;
  }

  operand_t src1, src2, dest;

  if (!parse_operand(src1_str, &src1) || !parse_operand(src2_str, &src2) ||
      !parse_operand(dest_str, &dest)) {
    return NULL;
  }

  instruction_t* instr = NULL;
  if (strcmp(instr_name, "addcc") == 0) {
    instr = addcc(src1, src2, dest);
  } else if (strcmp(instr_name, "andcc") == 0) {
    instr = andcc(src1, src2, dest);
  } else if (strcmp(instr_name, "orcc") == 0) {
    instr = orcc(src1, src2, dest);
  } else if (strcmp(instr_name, "orncc") == 0) {
    instr = orncc(src1, src2, dest);
  } else if (strcmp(instr_name, "srl") == 0) {
    instr = srl(src1, src2, dest);
  } else if (strcmp(instr_name, "nop") == 0) {
    instr = nop();
  } else {
    return NULL;
  }

  file->position++;
  return instr;
}

void close_file(file_t* file) {
  if (file && file->fp) {
    fclose(file->fp);
  }
  free(file);
}
