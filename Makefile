
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Iinclude

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, build/%.o,$(SRC))
DEP := $(OBJ:%.o=%.d)

EXE := build/app
LIBS :=

all : debug

debug: CFLAGS += -g
debug: $(EXE)

remake : clean debug
.NOTPARALLEL : remake

clean :
	rm -rf build/*

$(EXE) : $(OBJ) | build
	$(CC) -o $@ $^ $(LIBS)

build/%.o : src/%.c | build
	$(CC) $(CFLAG) -c -o $@ $<

build:
	mkdir -p build

-include $(DEP)

