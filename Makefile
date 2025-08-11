
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c89 -Iinclude
LDFLAGS =  
ASAN_FLAGS = -fsanitize=address

VALGRIND_FLAGS = --leak-check=full -s

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, build/%.o,$(SRC))
DEP := $(OBJ:%.o=%.d)

EXE := build/app
LIBS := -lpthread

all : debug

debug: CFLAGS += -g $(ASAN_FLAGS)
debug: LDFLAGS += $(ASAN_FLAGS)
debug: clean build $(EXE)

gdb: debug
	gdb --tui $(EXE)

remake : clean debug
.NOTPARALLEL : remake

run: debug
	./$(EXE)

valgrind: debug
	valgrind $(VALGRIND_FLAGS) $(EXE)

clean :
	rm -rf build/*

$(EXE) : $(OBJ) | build
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

build/%.o : src/%.c | build
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p build

-include $(DEP)

