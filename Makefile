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

UI_SRC_FILES = ui/arcvm_ui.c \
	ui/components/disassembly.c \
	ui/components/registers.c \
	ui/components/terminal.c

INCLUDE_DIRS = -I. -Ivm -Iutils -Iinstruction_set -Iinstruction_set/arithmetic_instructions -Iinstruction_set/logic_instructions -Iinstruction_set/shift_instructions

TARGET = exec.out
UI_TARGET = arcvm_ui.out

all:
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o $(TARGET)
.PHONY: all

ui:
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) $(UI_SRC_FILES) -lncurses -o $(UI_TARGET)
.PHONY: ui

clean:
	rm -f $(TARGET) $(UI_TARGET)
.PHONY: clean