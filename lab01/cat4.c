// COMP1521 19T2 ... lab 1
// cat3: Copy input to output

#include <stdio.h>
#include <stdlib.h>

static void copy (FILE *, FILE *);

int main (int argc, char *argv[])
{
    if (argc == 1) {
        copy (stdin, stdout);
    }
    else {
        for (int x = 1; x != argc; x++) {
            if (fopen (argv[x], "r") == NULL) {
                printf ("Can't Read %s", argv[x]);
            }
            else {
                FILE *f = fopen (argv[x], "r");
                copy (f, stdout);
                fclose (f);
            }  
        }
    }         
	return EXIT_SUCCESS;
}

// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode
static void copy (FILE *input, FILE *output)
{
    char c[BUFSIZ];
    
    while (fgets(c, BUFSIZ, input) != NULL) {
    fputs(c, output);
    }
}
