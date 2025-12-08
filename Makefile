CC = gcc
CC_FLAGS = -Wall -Wextra -g -std=c11

SRC_FILES = main.c \
	compiler.c \
	utils/hash.c \
	vm/VM.c \
	vm/storage/memory.c \
	vm/program.c \
	instruction_set/instruction_set.c \
	instruction_set/nop.c \
	instruction_set/arithmetic_instructions/addcc.c \
	instruction_set/logic_instructions/andcc.c \
	instruction_set/logic_instructions/orcc.c \
	instruction_set/logic_instructions/orncc.c \
	instruction_set/shift_instructions/srl.c

INCLUDE_DIRS = -I. -Ivm -Iutils -Iinstruction_set -Iinstruction_set/arithmetic_instructions -Iinstruction_set/logic_instructions -Iinstruction_set/shift_instructions

TARGET = exec.out

all:
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o $(TARGET)
.PHONY: all

clean:
	rm -f $(TARGET)
.PHONY: clean