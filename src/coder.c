
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

int read_next_code_unit (FILE *in, CodeUnits *code_units)
{
	char *buffer = (char *) calloc (sizeof (char), 9);
	char *buf = (char *) calloc (sizeof (char), 1);

	do {
		*buf = fgetc (in);
		if (*buf == '1' || *buf == '0'){

			fseek (in, -1, SEEK_CUR);
			if (fgets (buffer, 9, in) == NULL) {
				free (buffer);
				free (buf);
				return -1;
			}

			if (*buffer == '1' && buffer[1] == '0') {
				continue;
			}

			if (buffer[0] == '0') {

				code_units->length = 1;

				for (int j = 0; j < code_units->length; j++) {
					for (int i = 0; i < 8; i++) {
						if (buffer [i + j * 8] == '1') {
							code_units->code[j] = (code_units->code[j] << 1)\
							 												| 1;
						} else {
							code_units->code[j] = code_units->code[j] << 1;
						}
					}
				}

				free (buffer);
				free (buf);
				return 0;
			}

			if ((strncmp (buffer, "11", 2) == 0) && buffer[2] == '0') {
				char code [16];

				memcpy(code, buffer, 8);

				fseek (in, 1, SEEK_CUR);
				if (fgets (buffer, 9, in) == NULL) {
					free (buffer);
					free (buf);
					return -1;
				}

				if (buffer[0] == '1' && buffer[1] == '0') {
					memcpy(code + 8, buffer, 8);
				} else {
					fseek (in, -8, SEEK_CUR);
					free (buffer);
					free (buf);
					return -1;
				}

				code_units->length = 2;

				for (int j = 0; j < code_units->length; j++) {
					for (int i = 0; i < 8; i++) {
						if (code [i + j * 8] == '1') {
							code_units->code[j] = (code_units->code[j] << 1)\
							 												| 1;
						} else {
							code_units->code[j] = code_units->code[j] << 1;
						}
					}
				}

				free (buffer);
				free (buf);
				return 0;
			}

			if ((strncmp (buffer, "111", 3) == 0) && buffer[3] == '0'){
				char code [24];

				memcpy(code, buffer, 8);

				for (int i = 1; i <= 2; i++) {
					fseek (in, 1, SEEK_CUR);
					if (fgets (buffer, 9, in) == NULL) {
						free (buffer);
						free (buf);
						return -1;
					}

					if (buffer[0] == '1' && buffer[1] == '0') {
						memcpy(code + (i*8), buffer, 8);
					} else {
						fseek (in, -8, SEEK_CUR);
						free (buffer);
						free (buf);
						return -1;
					}
				}

				code_units->length = 3;

				for (int j = 0; j < code_units->length; j++) {
					for (int i = 0; i < 8; i++) {
						if (code [i + j * 8] == '1') {
							code_units->code[j] = (code_units->code[j] << 1)\
							 												| 1;
						} else {
							code_units->code[j] = code_units->code[j] << 1;
						}
					}
				}

				free (buffer);
				free (buf);
				return 0;
			}

			if ((strncmp (buffer, "1111", 4) == 0) && buffer[4] == '0'){

				char code[32];

				memcpy(code, buffer, 8);

				for (int i = 1; i <= 3; i++) {
					fseek (in, 1, SEEK_CUR);
					if (fgets (buffer, 9, in) == NULL) {
						free (buffer);
						free (buf);
						return -1;
					}

					if (buffer[0] == '1' && buffer[1] == '0') {
						memcpy(code + (i*8), buffer, 8);
					} else {
						fseek (in, -8, SEEK_CUR);
						free (buffer);
						free (buf);
						return -1;
					}
				}

				code_units->length = 4;

				for (int j = 0; j < code_units->length; j++) {
					for (int i = 0; i < 8; i++) {
						if (code [i + j * 8] == '1') {
							code_units->code[j] = (code_units->code[j] << 1)\
																			| 1;
						} else {
							code_units->code[j] = code_units->code[j] << 1;
						}
					}
				}

				free (buffer);
				free (buf);
				return 0;
			}

		}
	} while (*buf != EOF);

	free (buffer);
	free (buf);
	return -2;
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

int write_code_unit (FILE *out, const CodeUnits *code_units)
{
	for (int j = 0; j < code_units->length; j++) {
		for (uint8_t i = 128; i != 0; i = i >> 1){
			(code_units->code[j] & i) ? fputc ('1', out) : fputc ('0', out);

			if ( ferror (out)) {
				return -1;
			} else {
				continue;
			}
		}
		(j != (code_units->length - 1)) ? fputc (' ', out) : fputc ('\n', out);

		if ( ferror (out)) {
			return -1;
		} else {
			continue;
		}
	}

	return 0;
}
