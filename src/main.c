
#include "theme.h"
#include "coder.h"
#include <stdlib.h>

int main (){
	system ("clear");

	CodeUnits *b = malloc (sizeof (CodeUnits));
	FILE *in;

	in = fopen ("test.txt", "r");

	if (b == NULL) {
		printf (ERROR "Error of initialization\n" END);
		return -1;
	}

	for (int a = 0; a != 1;) {
		a = read_next_code_unit (in, b);
		if (a == 0) {
			print_code (b);
		} else if (a == -1) {
			printf(ERROR "ERROR\n" END);
		} else if (a == -2) {
			printf(INFO "EOF\n" END);
			break;
		}
	}

	fclose (in);

	return 0;
}
