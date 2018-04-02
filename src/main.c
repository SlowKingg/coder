
#include "coder.h"
#include <stdlib.h>
#include <stdio.h>

int main ()
{
	system ("clear");

	uint32_t a = 0;
	CodeUnits *b = NULL;

	printf ("Enter the value: \n");
	scanf ("%u", &a);
	printf ("\n");

	encode (a, b);

	return 0;
}
