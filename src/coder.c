#define _BSD_SOURCE
#include "theme.h"
#include "coder.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


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
	int pos = 0;
	code_units->length = 0;
	for (int i = 0; i < MaxCodeLength; i++) {
		code_units->code [i] = code_units->code [i] & 0;
	}

	for (uint32_t i = 1; i != 0; i = i << 1){
		pos++;

		if ((code_point & i) != 0) {
			sbit = pos;
		}
	}

	if (sbit > 21) {
		printf (ERROR "Value is too large\n" END);
		return -1;
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

int read_next_code_unit (FILE *in, CodeUnits *code_units) {

	int exflag = -1;

	if (exflag == -1) {
		char path [255];
    	char lnk [255];
    	int fno;
    	ssize_t r;

    	if (in != NULL) {
    		fno = fileno(in);

        	sprintf(lnk, "/proc/self/fd/%d", fno);

        	r = readlink(lnk, path, 255);

    		if (r < 0) {
            	printf("failed to readlink\n");
            	return -1;
        	}

        	path[r] = '\0';
    	}

    	if (strstr (path, ".txt\0") != NULL) {
			exflag = 1;
		} else if (strstr (path, ".bin\0") != NULL) {
			exflag = 0;
		} else {
    		return -1;
		}
	}

	if (exflag == 1) {

		char ch;

		do {
			ch = getc (in);

			if (ch == EOF) {
				return -2;
			}
		} while (ch == ' ' || ch == '\n');

		if (fseek (in, -1, SEEK_CUR)) {
			return -1;
		}

		char code [8];
		uint32_t num;

		if (fgets (code, 8, in) == NULL) {
			if (feof (in)) {
				return -2;
			}

			if (ferror (in)) {
				return -1;
			}
		}

		num = strtol (code, NULL, 16);

		if (encode (num, code_units)) {
			return -1;
		}

		return 0;
	}

	if (exflag == 0) {

		CodeUnits *form = calloc (sizeof (CodeUnits), 1);
		if (form == NULL) {
			return -1;
		}

		uint8_t *byte = calloc (sizeof (uint8_t), 1);

		read: if (fread (byte, sizeof (uint8_t), 1, in) != 1) {
			if (feof (in)) {
				free (byte);
				free (form);
				return -2;
			}

			if (ferror (in)) {
				free (byte);
				free (form);
				return -1;
			}
		}

		if ((*byte & 0xc0) == 0x80) {
			goto read;
		}

		if ((*byte & 0x80) == 0) {
			form->code [0] = *byte;
			form->length = 1;
		} else if ((*byte & 0xe0) == 0xc0) {
			form->code [0] = *byte;
			form->length = 2;

			uint8_t *cont = calloc (sizeof (uint8_t), 1);
			if (cont == NULL) {
				return -1;
			}

			for (int i = 1; i < form->length; i++) {
				if (fread (cont, sizeof (uint8_t), 1, in) != 1) {
					if (feof (in)) {
						free (byte);
						free (form);
						return -2;
					}
					if (ferror (in)) {
						free (byte);
						free (form);
						return -1;
					}
				}

				if ((*cont & 0xc0) == 0x80) {
					form->code [i] = *cont;
				} else {
					if (fseek (in, -1, SEEK_CUR)) {
						free (byte);
						free (form);
						return -1;
					}
					goto read;
				}
			}
		} else if ((*byte & 0xf0) == 0xe0) {
			form->code [0] = *byte;
			form->length = 3;

			uint8_t *cont = calloc (sizeof (uint8_t), 1);
			if (cont == NULL) {
				return -1;
			}

			for (int i = 1; i < form->length; i++) {
				if (fread (cont, sizeof (uint8_t), 1, in) != 1) {
					if (feof (in)) {
						free (byte);
						free (form);
						return -2;
					}
					if (ferror (in)) {
						free (byte);
						free (form);
						return -1;
					}
				}

				if ((*cont & 0xc0) == 0x80) {
					form->code [i] = *cont;
				} else {
					if (fseek (in, -1, SEEK_CUR)) {
						free (byte);
						free (form);
						return -1;
					}
					goto read;
				}
			}
		} else if ((*byte & 0xf8) == 0xf0) {
			form->code [0] = *byte;
			form->length = 4;

			uint8_t *cont = calloc (sizeof (uint8_t), 1);
			if (cont == NULL) {
				return -1;
			}

			for (int i = 1; i < form->length; i++) {
				if (fread (cont, sizeof (uint8_t), 1, in) != 1) {
					if (feof (in)) {
						free (byte);
						free (form);
						return -2;
					}
					if (ferror (in)) {
						free (byte);
						free (form);
						return -1;
					}
				}

				if ((*cont & 0xc0) == 0x80) {
					form->code [i] = *cont;
				} else {
					if (fseek (in, -1, SEEK_CUR)) {
						free (byte);
						free (form);
						return -1;
					}
					goto read;
				}
			}
		}

		for (int i = 0; i < MaxCodeLength; i++) {
			code_units->code[i] = form->code[i];
		}
		code_units->length = form->length;

		free (form);
		free (byte);
		return 0;
	}

	return -1;
}

int write_code_unit (FILE *out, const CodeUnits *code_units) {

	int exflag = -1;

	if (exflag == -1) {
		char path [255];
		char lnk [255];
		int fno;
		ssize_t r;

		if (out != NULL) {
			fno = fileno(out);

			sprintf(lnk, "/proc/self/fd/%d", fno);

			r = readlink(lnk, path, 255);

			if (r < 0) {
				printf("failed to readlink\n");
				return -1;
			}

			path[r] = '\0';
		}

		if (strstr (path, ".txt\0") != NULL) {
			exflag = 1;
		} else if (strstr (path, ".bin\0") != NULL) {
			exflag = 0;
		} else {
			return -1;
		}
	}

	if (exflag == 1) {
		uint32_t code_point = 0;

		code_point = decode (code_units);

		fprintf(out, "%x\n", code_point);

		return 0;
	}

	if (exflag == 0) {
		if (code_units->length == 1) {
			fwrite (&code_units->code[0], 1, 1, out);

			return 0;
		} else if (code_units->length == 2) {
			for (int i = 0; i < code_units->length; i++) {
				if (fwrite (&code_units->code[i], 1, 1, out) != 1) {
					return -1;
				}
			}

			return 0;
		} else if (code_units->length == 3) {
			for (int i = 0; i < code_units->length; i++) {
				if (fwrite (&code_units->code[i], 1, 1, out) != 1) {
					return -1;
				}
			}

			return 0;
		} else if (code_units->length == 4) {
			for (int i = 0; i < code_units->length; i++) {
				if (fwrite (&code_units->code[i], 1, 1, out) != 1) {
					return -1;
				}
			}

			return 0;
		}
	}

	return -1;
}
