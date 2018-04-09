
#include "coder.h"
#include "command.h"
#include "theme.h"
#include <string.h>
#include <stdlib.h>

int encode_file(const char *in_file_name, const char *out_file_name) {

    FILE *in;
    FILE *out;

    in = fopen ( in_file_name, "rt");
    if (in == NULL) {
        printf (ERROR "Can't open" CBOLD " %s\n" END, in_file_name);

        return -1;
    }

    out = fopen ( out_file_name, "wb");
    if (out == NULL) {
        printf (ERROR "Can't open" CBOLD " %s\n" END, out_file_name);

        if (fclose (in)) {
            printf (ERROR "Can't properly close" CBOLD " %s\n" END,\
                                                                in_file_name);
        }

        return -1;
    }

    CodeUnits *code = calloc (sizeof (CodeUnits), 1);
    if (code == NULL) {
        printf (ERROR "Error of initialization\n" END);
        return -1;
    }
    
    for (int flag = 0;;) {
    
    	char ch;

		do {
			ch = getc (in);

			if (ch == EOF) {
				flag = -2;
			}
		} while (ch == ' ' || ch == '\n');

		if (fseek (in, -1, SEEK_CUR)) {
			flag = -1;
		}

		char code_s [8];
		uint32_t num;

		if (fgets (code_s, 8, in) == NULL) {
			if (feof (in)) {
				flag = -2;
			}

			if (ferror (in)) {
				flag = -1;
			}
		}

		num = strtol (code_s, NULL, 16);

		if (encode (num, code)) {
			flag = -1;
		}
		
		if (flag == 0) {
			if (write_code_unit (out, code)) {
				printf(ERROR "OUTPUT ERROR \n" END);
			}
		} else if (flag == -1) {
			printf(ERROR "ERROR\n" END);
			break;
		} else if (flag == -2) {
            printf(INFO "End of" CBOLD " %s " CINFO "reached\n" END,\
                                                                in_file_name);
			break;
		}
	}

    free (code);

    if (fclose (in)) {
        printf (ERROR "Can't properly close" CBOLD " %s\n" END, in_file_name);

        if (fclose (out)) {
            printf (ERROR "Can't properly close" CBOLD " %s\n" END,\
                                                                out_file_name);

        return -1;
        }
	}

    if (fclose (out)) {
        printf (ERROR "Can't properly close" CBOLD " %s\n" END, out_file_name);

        return -1;
    }


    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name) {

    FILE *in;
    FILE *out;

    in = fopen ( in_file_name, "rb");
    if (in == NULL) {
        printf (ERROR "Can't open" CBOLD " %s\n" END, in_file_name);

        return -1;
    }

    out = fopen ( out_file_name, "wt");
    if (out == NULL) {
        printf (ERROR "Can't open" CBOLD " %s\n" END, out_file_name);

        if (fclose (in)) {
            printf (ERROR "Can't properly close" CBOLD " %s\n" END,\
                                                                in_file_name);
        }

        return -1;
    }

    CodeUnits *code = calloc (sizeof (CodeUnits), 1);
    if (code == NULL) {
        printf (ERROR "Error of initialization\n" END);
        return -1;
    }


    for (int a = 0;;) {
		a = read_next_code_unit (in, code);
		if (a == 0) {
			uint32_t code_point = 0;

			code_point = decode (code);

			fprintf(out, "%x\n", code_point);
		} else if (a == -1) {
			printf(ERROR "ERROR\n" END);
			break;
		} else if (a == -2) {
			printf(INFO "End of" CBOLD " %s " CINFO "reached\n" END,\
                                                                in_file_name);
			break;
		}
	}

    free (code);

    if (fclose (in)) {
        printf (ERROR "Can't properly close" CBOLD " %s\n" END, in_file_name);

        if (fclose (out)) {
            printf (ERROR "Can't properly close" CBOLD " %s\n" END,\
                                                                out_file_name);

        return -1;
        }
	}

    if (fclose (out)) {
        printf (ERROR "Can't properly close" CBOLD " %s\n" END, out_file_name);

        return -1;
    }


    return 0;
}
