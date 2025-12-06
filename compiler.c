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

instruction_t* fetch(file_t* file) {
  if (!file) {
    return NULL;
  }

  char line[128];
  if (fgets(line, sizeof(line), file->fp) == NULL) {
    return NULL;
  }

  char instr_name[16];
  int src1, src2;
  char dest_reg[8];

  if (sscanf(line, "%15s %d, %d, %7s", instr_name, &src1, &src2, dest_reg) != 4) {
    return NULL;
  }

  int dest = -1;
  if (strcmp(dest_reg, "%r1") == 0) {
    dest = 0; // R1 is 0
  } else {
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
