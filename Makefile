SRC_FILES = main.c instruction_set.c instruction_set.h compiler.c compiler.h VM.c VM.h program.c program.h
CC_FLAGS = -Wall -Wextra -g -std=c11
TARGET = exec.out
CC = gcc

all:
	$(CC) $(CC_FLAGS) $(SRC_FILES) -o $(TARGET)
.PHONY: all