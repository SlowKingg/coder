
#include "theme.h"
#include "coder.h"
#include <stdlib.h>

int main (){
	system ("clear");

	uint32_t num = 0;
	CodeUnits *b = malloc (sizeof (CodeUnits));

	if (b == NULL) {
		printf (ERROR "Error of initialization\n" END);
		return -1;
	}

	FILE *in;
	in = fopen ("testin.txt", "r");
	if (in == NULL) {
		printf (ERROR "Can't open input file\n" END);
		return -1;
	}


	FILE *out;
	out = fopen ("testout.txt", "w");
	if (out == NULL) {
		printf (ERROR "Can't open output file\n" END);
		return -1;
	}

	for (int a = 0; a != 1;) {
		a = read_next_code_unit (in, b);
		if (a == 0) {
			print_code (b);
			num = decode (b);
			printf(INFO "The decoded code is" CBOLD " %i\n\n" END, num);
			if (write_code_unit (out, b)) {
				printf(ERROR "FILE ERROR\n" END);
			}
		} else if (a == -1) {
			printf(ERROR "ERROR\n" END);
		} else if (a == -2) {
			printf(INFO "EOF\n" END);
			break;
		}
	}

	if (fclose (in)) {
		printf (ERROR "Can't properly close input file\n" END);
	}

	if (fclose (out)) {
		printf (ERROR "Can't properly close output file\n" END);
	}

	return 0;
}
