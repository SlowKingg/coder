C = gcc
flag = -c -Wall -Werror
testflag = -Wall -Werror -g -O0

all: coder
coder: make Main.o Coder.o
	$(C) ./build/main.o ./build/coder.o -o ./bin/coder
	$(C) $(testflag) ./src/main.c ./src/coder.c -o ./bin/codertest

Main.o:
	$(C) $(flag) ./src/main.c -o ./build/main.o
	
Coder.o:
	$(C) $(flag) ./src/coder.c -o ./build/coder.o

make:
	mkdir -p bin
	mkdir -p build

.PHONY: clean
clean:
	rm -rf ./build/*.o
