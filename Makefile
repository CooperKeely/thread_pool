
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude

VALGRINDFLAGS = --leak-check=full -s

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, build/%.o,$(SRC))
DEP := $(OBJ:%.o=%.d)

EXE := build/app
LIBS :=

all : debug

debug: CFLAGS += -g
debug: clean build $(EXE)

gdb: debug
	gdb $(EXE)

remake : clean debug
.NOTPARALLEL : remake

gdb: debug
	gdb $(EXE)

run: debug
	./$(EXE)

valgrind: debug
	valgrind $(VALGRINDFLAGS) $(EXE)

clean :
	rm -rf build/*

$(EXE) : $(OBJ) | build
	$(CC) -o $@ $^ $(LIBS)

build/%.o : src/%.c | build
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p build

-include $(DEP)

