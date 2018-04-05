
#include "theme.h"
#include "coder.h"
#include <stdlib.h>

int main (){
	system ("clear");

	CodeUnits *b = calloc (sizeof (CodeUnits), 1);
	if (b == NULL) {
		printf (ERROR "Error of initialization\n" END);
		return -1;
	}

	FILE *inb;
	FILE *intext;
	FILE *outb;
	FILE *outtext;

	inb = fopen ("testin.bin", "rb");
	if (inb == NULL) {
		printf (ERROR "Can't open input file(BIN)\n" END);
		return -1;
	}

	intext = fopen ("testin.txt", "rt");
	if (intext == NULL) {
		printf (ERROR "Can't open input file(TXT)\n" END);
		return -1;
	}

	outtext = fopen ("testout.txt", "wt");
	if (intext == NULL) {
		printf (ERROR "Can't open output file(TXT)\n" END);
		return -1;
	}

	outb = fopen ("testout.bin", "wt");
	if (intext == NULL) {
		printf (ERROR "Can't open output file(BIN)\n" END);
		return -1;
	}


	for (int a = 0;;) {
		a = read_next_code_unit (inb, b);
		if (a == 0) {
			if (write_code_unit (outb, b)) {
				printf(ERROR "OUTPUT ERROR BIN\n" END);
			}
		} else if (a == -1) {
			printf(ERROR "ERROR BIN\n" END);
			break;
		} else if (a == -2) {
			printf(INFO "EOF OF BIN\n" END);
			break;
		}
	}

	for (int a = 0;;) {
		a = read_next_code_unit (intext, b);
		if (a == 0) {
			if (write_code_unit (outtext, b)) {
				printf(ERROR "OUTPUT ERROR TXT\n" END);
			}
		} else if (a == -1) {
			printf(ERROR "ERROR TXT\n" END);
			break;
		} else if (a == -2) {
			printf(INFO "EOF OF TXT\n" END);
			break;
		}
	}

	if (fclose (inb)) {
		printf (ERROR "Can't properly close input file(BIN)\n" END);
	}

	if (fclose (intext)) {
		printf (ERROR "Can't properly close input file(TXT)\n" END);
	}

	if (fclose (outtext)) {
		printf (ERROR "Can't properly close output file(TXT)\n" END);
	}

	if (fclose (outb)) {
		printf (ERROR "Can't properly close output file(BIN)\n" END);
	}

	return 0;
}
