
#include "coder.h"
#include <stdlib.h>
#include <stdio.h>

int main ()
{
	system ("clear");

	uint32_t a = 0;
	printf("Enter the value: \n");
	scanf("%u", &a);
	CodeUnits *b = NULL;

	encode (a, b);

	return 0;
}
