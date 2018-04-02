
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

int encode (uint32_t code_point, CodeUnits *code_units) {

	code_units = malloc (sizeof (CodeUnits));

	if (code_units == NULL) {
		printf (ERROR "Error of initialization\n" END);
		return -1;
	}

	int sbit = 0;
	int j = 0;

	for (uint32_t i = 1; i != 0; i = i << 1){
		j++;

		if ((code_point & i) != 0) {
			sbit = j;
		}

		if (sbit > 21) {
			printf (ERROR "Value is too large\n" END);
			return -1;
		}
	}

	printf (INFO "Number of significant bits:" CBOLD " %d\n" END, sbit);

	if (sbit <= 7) {
		code_units->length = 1;
		} else if (sbit > 7 && sbit <= 11 ) {
		code_units->length = 2;
		} else if (sbit > 11 && sbit <= 16 ) {
		code_units->length = 3;
		} else if (sbit > 16 && sbit <= 21 ) {
		code_units->length = 4;
	}

	printf (INFO "Number of bytes in the code:" CBOLD " %lu\n" END,\
	 													code_units->length);

	if (code_units->length == 1) {
		code_units->code[0] = code_point & 0x7F;
	} else if (code_units->length == 2) {
		code_units->code[0] = ((code_point >> 6) & 0x1F) | 0xC0;
		code_units->code[1] = (code_point & 0x3F ) | 0x80;
	} else if (code_units->length == 3) {
		code_units->code[0] = ((code_point >> 12) & 0xF) | 0xE0;
		code_units->code[1] = ((code_point >> 6) & 0x3F ) | 0x80;
		code_units->code[2] = (code_point & 0x3F ) | 0x80;
	} else if (code_units->length == 4) {
		code_units->code[0] = ((code_point >> 18) & 0x7) | 0xF0;
		code_units->code[1] = ((code_point >> 12) & 0x3F ) | 0x80;
		code_units->code[2] = ((code_point >> 6) & 0x3F ) | 0x80;
		code_units->code[3] = (code_point & 0x3F ) | 0x80;
	}

	for (j = 0; j < MaxCodeLength; j++) {
		for (uint8_t i = 128; i != 0; i = i >> 1){
			(code_units->code[j] & i) ? printf (CBOLD "1") : printf (CBOLD "0");
		}
		(j != (MaxCodeLength - 1)) ? printf (CCORRECT "_") :\
		 												printf (END "\n");
	}

	return 0;
}
