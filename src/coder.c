
#include "theme.h"
#include "coder.h"
#include <string.h>
#include <stdlib.h>

void print_code (CodeUnits *code_units)
{
	for (int j = 0; j < MaxCodeLength; j++) {
		for (uint8_t i = 128; i != 0; i = i >> 1){
			(code_units->code[j] & i) ? printf (CBOLD "1") : printf (CBOLD "0");
		}
		(j != (MaxCodeLength - 1)) ? printf (CCORRECT "_") :\
															printf (END "\n");
	}

	printf(INFO "Number of bytes in code:" CBOLD " %lu\n" END,\
	 														code_units->length);
}

uint32_t decode (const CodeUnits *code_units)
{
	uint32_t code_point = 0;

	if (code_units->length == 1) {
		code_point = code_point | code_units->code [0];
	} else if (code_units->length == 2) {
		for (int i = 0; i < code_units->length; i++) {
			if (i == 0) {
				code_point = code_point | (code_units->code [i] & 0x1F);
			} else {
				code_point = code_point << 6;
				code_point = code_point | (code_units->code [i] & 0x3F);
			}
		}
	} else if (code_units->length == 3) {
		for (int i = 0; i < code_units->length; i++) {
			if (i == 0) {
				code_point = code_point | (code_units->code [i] & 0xF);
			} else {
				code_point = code_point << 6;
				code_point = code_point | (code_units->code [i] & 0x3F);
			}
		}
	} else if (code_units->length == 4) {
		for (int i = 0; i < code_units->length; i++) {
			if (i == 0) {
				code_point = code_point | (code_units->code [i] & 0x7);
			} else {
				code_point = code_point << 6;
				code_point = code_point | (code_units->code [i] & 0x3F);
			}
		}
	}

	return code_point;
}

int encode (uint32_t code_point, CodeUnits *code_units)
{

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

	if (sbit <= 7) {
		code_units->length = 1;
		} else if (sbit > 7 && sbit <= 11 ) {
		code_units->length = 2;
		} else if (sbit > 11 && sbit <= 16 ) {
		code_units->length = 3;
		} else if (sbit > 16 && sbit <= 21 ) {
		code_units->length = 4;
	}

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


	return 0;
}
