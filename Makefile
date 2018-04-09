C = gcc
flag = -c -Wall -Werror -std=c99
testflag = -Wall -Werror -std=c99 -g -O0

all: coder
coder: make Main.o Coder.o Command.o
	$(C) ./build/main.o ./build/coder.o ./build/command.o -o ./bin/coder
	$(C) $(testflag) ./src/main.c ./src/coder.c ./src/command.c -o ./bin/codertest

Main.o:
	$(C) $(flag) ./src/main.c -o ./build/main.o

Coder.o:
	$(C) $(flag) ./src/coder.c -o ./build/coder.o

Command.o:
	$(C) $(flag) ./src/command.c -o ./build/command.o

make:
	mkdir -p bin
	mkdir -p build

.PHONY: clean
clean:
	rm -rf ./build/*.o
