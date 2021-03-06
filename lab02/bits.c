
// COMP1521 19t2 ... lab 2 warm-up

#include <stdio.h>
#include <stdlib.h>

static void range_uchar (void);
static void range_char (void);
static void range_uhint (void);
static void range_hint (void);
static void range_int (void);
static void range_uint (void);
static void range_lint (void);
static void range_ulint (void);
static void range_llint (void);
static void range_ullint (void);

int main (void)
{
	range_char ();
	range_uchar ();
	range_hint ();
	range_uhint ();
	range_int ();
	range_uint ();
	range_lint ();
	range_ulint ();
	range_llint ();
	range_ullint ();

	return EXIT_SUCCESS;
}

// Calculate and print the largest and smallest `int` values
static void range_int (void)
{
	int int_min = 1 << (8 * sizeof(int) - 1); 
	int int_max = ~(1 << (8 * sizeof(int) - 1));
	printf (
		"                   int (%zu bytes): %d ... %d\n",
		sizeof (int), int_min, int_max
	);
}

// Calculate and print the largest and smallest `unsigned int` values
static void range_uint (void)
{
	unsigned int uint_min = 0;
	unsigned int uint_max = ~0;
	printf (
		"          unsigned int (%zu bytes): %u ... %u\n",
		sizeof (unsigned int), uint_min, uint_max
	);
}

// Calculate and print the largest and smallest `long int` values
static void range_lint (void)
{

    // 1 == ...0000 0000 0000 0000 0001 32 its
    // 1L == ...0000 0000 0000 0000 0001 64 bits

	long int long_min = 1L << (8 * sizeof(long int) - 1);
	long int long_max = ~(1L << (8 * sizeof(long int) - 1));
	printf (
		"              long int (%zu bytes): %ld ... %ld\n",
		sizeof (long int), long_min, long_max
	);
}

// Calculate and print the largest and smallest `unsigned long int` values
static void range_ulint (void)
{
	unsigned long int ulong_min = 0;
	unsigned long int ulong_max = ~0;
	printf (
		"     unsigned long int (%zu bytes): %lu ... %lu\n",
		sizeof (unsigned long int), ulong_min, ulong_max
	);
}

// Calculate and print the largest and smallest `long long int` values
static void range_llint (void)
{
	long long int llong_min = 1LL << (8 * sizeof(long long int) - 1);
	long long int llong_max = ~(1LL << (8 * sizeof(long long int) - 1)  );
	printf (
		"         long long int (%zu bytes): %lld ... %lld\n",
		sizeof (long long int), llong_min, llong_max
	);
}

// Calculate and print the largest and smallest `unsigned long long int` values
static void range_ullint (void)
{
	unsigned long long int ullong_min = 0;
	unsigned long long int ullong_max = ~0;
	printf (
		"unsigned long long int (%zu bytes): %llu ... %llu\n",
		sizeof (unsigned long long int), ullong_min, ullong_max
	);
}

// Calculate and print the largest and smallest `short int` values
static void range_hint (void)
{
	short int hint_min = 1 << (8 * sizeof(short int) - 1);
	short int hint_max =  ~hint_min;
	printf (
		"             short int (%zu bytes): %hi ... %hi\n",
		sizeof (short int), hint_min, hint_max
	);
}

// Calculate and print the largest and smallest `unsigned short int` values
static void range_uhint (void)
{
	unsigned short int uhint_min = 0; 
	unsigned short int uhint_max = ~(0); 
	printf (
		"    unsigned short int (%zu bytes): %hu ... %hu\n",
		sizeof (unsigned short int), uhint_min, uhint_max
	);
}

// Calculate and print the largest and smallest `char` values
static void range_char (void)
{
	char char_min = 1 << (8 * sizeof(char) - 1);
	//char char_max = ~(1 << (8 * sizeof(char) - 1));
	printf (
		"                  char (%zu bytes): %c ... \n",
		sizeof (char), char_min
	);
}

// Calculate and print the largest and smallest `unsigned char` values
static void range_uchar (void)
{
	unsigned char uchar_min = 0;
	unsigned char uchar_max = ~(0);
	printf (
		"         unsigned char (%zu bytes): %u ... %u\n",
		sizeof (unsigned char), uchar_min, uchar_max
	);
}
