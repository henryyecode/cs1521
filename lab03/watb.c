// COMP1521 19t2 ... lab 03: where are the bits?
// watb.c: determine bit-field order

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
	unsigned int a : 4;
	unsigned int b : 8;
	unsigned int c : 20;
};

union bit_union {
    int field1;
    struct _bit_fields field2;
};

int main (void)
{
	struct _bit_fields x;
	union bit_union y;
	y.field2.a = 1;
	y.field2.b = 0;
	y.field2.c = 0;
	printf ("%d\n", y.field1);

	printf ("%zu\n", sizeof (x));

	return EXIT_SUCCESS;
}
