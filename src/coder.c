
#include "coder.h"
#include <stdlib.h>
#include <stdio.h>

#define ERROR "\e[31;1;4;2m"
#define CERROR "\e[0;31;1;4;2m"
#define CORRECT "\e[32;1;4;2m"
#define CCORRECT "\e[0;32;1;4;2m"
#define INFO "\e[36;1;4m"
#define CINFO "\e[0;36;1;4m"
#define BOLD "\e[1m"
#define CBOLD "\e[0;1m"
#define END "\e[0m"

int encode(uint32_t code_point, CodeUnits *code_units) {

	code_units = malloc (sizeof (CodeUnits));

	if (code_units == NULL) {
		printf (ERROR "Error of initialization\n" END);
		return -1;
	}

	int sbit = 0;
	int j = 0;

	for (uint32_t i = 1; i <= 2147483648 && i != 0;i = i << 1){
		j++;

		if ((code_point & i) != 0) {
			sbit = j;
		}
	}

	if (sbit <= 7) {
		code_units->length = 1;
		printf(CORRECT "Number of bytes in the code: 1\n" END);
	} else if (sbit > 7 && sbit <= 11 ) {
		code_units->length = 2;
		printf(CORRECT "Number of bytes in the code: 2\n" END);
	} else if (sbit > 11 && sbit <= 16 ) {
		code_units->length = 3;
		printf(CORRECT "Number of bytes in the code: 3\n" END);
	} else if (sbit > 16 && sbit <= 21 ) {
		code_units->length = 4;
		printf(CORRECT "Number of bytes in the code: 4\n" END);
	} else if (sbit > 21) {
		printf(ERROR "Value is too large\n" END);
		return -1;
	}

	return 0;
}
