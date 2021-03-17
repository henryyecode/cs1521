// BigNum.c ... LARGE positive integer values

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BigNum.h"

// Initialise a BigNum to N bytes, all zero
void initBigNum (BigNum *bn, int Nbytes)
{
    bn.nbytes = Nbytes;
    bn->bytes = malloc(Nbytes * sizeof(int));
    for (int i = 0; i < bn.nbytes; i++) {
        bn->bytes[i] = 0;
    }
    assert(bn->bytes != NULL);
	// TODO
	return;
}

// Add two BigNums and store result in a third BigNum
void addBigNums (BigNum bnA, BigNum bnB, BigNum *res)
{
	
	// TODO
	int u = 0;
	if (bnA.nbytes > bnB.nbytes) {
	    u = bnA.nbytes;
	} else {
	    u = bnB.nbytes;
	}
	
	for (int i = 0; i < bnA.nbytes && i < bnB.nbytes; i++) {
	    res[u] = bnA->bytes[i] + bnB->bytes[i];
	    if (res[u] > 9) {
	        
	    u--;
	
	
	
	
	
	
	return;
}

// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum (char *s, BigNum *bn)
{
	// TODO
	int j = 0;
	
	while (!isdigit(s[i])
	for (int i = 0; i < strlen(s); i++) {
	    while (!isdigit(s[i])) {
	        i++;
	        
	    }
	    i++;
	}  
	return 1;
}

// Display a BigNum in decimal format
void showBigNum (BigNum bn)
{
	for (int i = bn.nbytes; i >= 0; i--) {
	    printf("%c", bn->bytes[i]);
	}
	    
	    
	// TODO
	return;
}
