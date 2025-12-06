SRC_FILES = main.c
CC_FLAGS = -Wall -Wextra -g -std=c11
TARGET = exec
CC = gcc

all:
  $(CC) $(CC_FLAGS) $(SRC_FILES) -o $(TARGET)
.PHONY: all