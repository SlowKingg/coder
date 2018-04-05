
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

	inb = fopen ("out.bin", "rb");
	if (inb == NULL) {
		printf (ERROR "Can't open input file(BIN)\n" END);
		return -1;
	}

	intext = fopen ("testin.txt", "rt");
	if (intext == NULL) {
		printf (ERROR "Can't open input file(TXT)\n" END);
		return -1;
	}

	printf(INFO "\t\n***BIN***\n\n" END);

	for (int a = 0;;) {
		a = read_next_code_unit (inb, b);
		if (a == 0) {
			print_code (b);
		} else if (a == -1) {
			printf(ERROR "ERROR BIN\n" END);
			break;
		} else if (a == -2) {
			printf(INFO "EOF OF BIN\n" END);
			break;
		}
	}

	printf(INFO "\t\n***TEXT***\n\n" END);

	for (int a = 0;;) {
		a = read_next_code_unit (intext, b);
		if (a == 0) {
			print_code (b);
		} else if (a == -1) {
			printf(ERROR "ERROR BIN\n" END);
			break;
		} else if (a == -2) {
			printf(INFO "EOF OF BIN\n" END);
			break;
		}
	}

	if (fclose (inb)) {
		printf (ERROR "Can't properly close input file(BIN)\n" END);
	}

	if (fclose (intext)) {
		printf (ERROR "Can't properly close input file(TXT)\n" END);
	}

	return 0;
}
