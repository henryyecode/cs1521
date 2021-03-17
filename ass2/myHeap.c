// COMP1521 19t2 ... Assignment 2: heap management system

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myHeap.h"

/** minimum total space for heap */
#define MIN_HEAP 4096
/** minimum amount of space for a free Chunk (excludes Header) */
#define MIN_CHUNK 32


#define ALLOC 0x55555555
#define FREE  0xAAAAAAAA

/// Types:

typedef unsigned int  uint;
typedef unsigned char byte;

typedef uintptr_t     addr; // an address as a numeric type

/** The header for a chunk. */
typedef struct header {
	uint status;    /**< the chunk's status -- ALLOC or FREE */
	uint size;      /**< number of bytes, including header */
	byte data[];    /**< the chunk's data -- not interesting to us */
} header;

/** The heap's state */
struct heap {
	void  *heapMem;     /**< space allocated for Heap */
	uint   heapSize;    /**< number of bytes in heapMem */
	void **freeList;    /**< array of pointers to free chunks */
	uint   freeElems;   /**< number of elements in freeList[] */
	uint   nFree;       /**< number of free chunks */
};


/// Variables:

/** The heap proper. */
static struct heap Heap;


/// Functions:

static addr heapMaxAddr (void);


/** Initialise the Heap. */
int initHeap (int size)
{
	Heap.nFree = 0;
	Heap.freeElems = 0;
    
    // If N < minimum heap size, then N is set to minimum heap size. 
	if (size < MIN_HEAP)
	   size = MIN_HEAP;
	// Rounded to nearest multiple of 4
	else {
	    int remainder = size % 4;
	    if (remainder != 0) {
	        size = size + ( 4 - remainder);
	    }     
	}
	
	Heap.heapSize = size;
	// Allocate Memory to heapMem 
	Heap.heapMem = malloc (Heap.heapSize);
	
	
	// fail returns -1
	if (Heap.heapMem == NULL) {
        return -1;
    }
    memset (Heap.heapMem, 0, Heap.heapSize);
	// initialise pointer to HeapMem
    header *h;
    h = (header *)Heap.heapMem;
    h->status = FREE;
    h->size = size;
    
    // initialise freeList Array, pointing to h
    Heap.freeElems = size / MIN_CHUNK;
    Heap.freeList = malloc (sizeof (header *) * Heap.freeElems);
	Heap.freeList[0] = h;
	Heap.nFree++;

	return 0; // this just keeps the compiler quiet
}

/** Release resources associated with the heap. */
void freeHeap (void)
{
	free (Heap.heapMem);
	free (Heap.freeList);
}

/** Allocate a chunk of memory large enough to store `size' bytes. */
void *myMalloc (int size)
{   
    // create void pointer to return
    void *p;
    
    if (size < 1) {
        return NULL;
    }
    
    // round up size to nearest 4
    int remainder = size % 4;
    if (remainder != 0) {
        size = size + (4 - remainder);
    } 
    
    // find the smallest FREE block larger than size
    uint smallest = Heap.heapSize;
    int index = -1;
    for (int i = 0; i < Heap.nFree; i++) {
        if (((header *)Heap.freeList[i])->size >= (size + sizeof (header))) {      
            if (((header *)Heap.freeList[i])->size <= smallest) {
                smallest = ((header *)Heap.freeList[i])->size;
                index = i;
            }
        }         
    }
    
    // if not found, return null
    if (index == -1) {
        return NULL;
    }
    // If the free chunk is smaller than N + HeaderSize + MIN_CHUNK, allocate the whole chunk
    if (((header *)Heap.freeList[index])->size < (size + sizeof(header) + MIN_CHUNK)) {
        p = (header *)(Heap.freeList[index] + sizeof(header));
        ((header *)Heap.freeList[index])->status = ALLOC;
        for ( ; index < Heap.nFree - 1; index++) {
            Heap.freeList[index] = Heap.freeList[index + 1];
        }
        Heap.freeList[index] = NULL;
        Heap.nFree--;
    }
    // else, split into 2 chunks
    else {
        uint chunk_size = ((header *)Heap.freeList[index])->size;
        ((header *)Heap.freeList[index])->size = size + sizeof (header *);
        ((header *)Heap.freeList[index])->status = ALLOC;
        p = (header *)((Heap.freeList[index]) + sizeof(header *));
        Heap.freeList[index] = (header *)((Heap.freeList[index]) + size + sizeof (header *));  
        ((header *)Heap.freeList[index])->status = FREE;
        ((header *)Heap.freeList[index])->size = chunk_size - size - sizeof (header *);
    } 
	return p;
}

/** Deallocate a chunk of memory. */
void myFree (void *obj)
{   
    // print to stderr if invalid argument
    header *h = (header *)(obj - sizeof(header));
    if (obj == NULL || h->status != ALLOC) {
        fprintf(stderr, "Attempt to free unallocated chunk\n");
        exit(1);
    }
    
    int position = 0;
    header *prevFree = NULL;
    header *nextFree = NULL;
    // 
    for (int i = 0; i < Heap.nFree; i++) {
        if ((addr)Heap.freeList[i] > (addr) h) {
            if (i == 0) {
                nextFree = (header *)(Heap.freeList[0]);
                break;
            }
            else {
                prevFree = (header *)(Heap.freeList[i - 1]);
                nextFree = (header *)(Heap.freeList[i]);
                position = i;
                break;
            }
        }
    }
    // object is after last free block 
    if (prevFree == NULL && nextFree == NULL) {
        nextFree = (header *)(Heap.freeList[Heap.nFree - 1]);
        //join to last free block if adjacent
        if (((addr)nextFree + nextFree->size) == (addr) h) {
            nextFree->size += h->size; 
            memset(h, 0, h->size);              
        }
        //else just free chunk
        else {
            h->status = FREE;
            memset(obj, 0, (h->size - sizeof(header *)));
            // add free chunk to list
            Heap.freeList[Heap.nFree] = h;
            Heap.nFree++;      
        }
    }
    // object is before first free block
    else if (prevFree == NULL) {
        h->status = FREE;
        prevFree = (header *)((addr)h + h->size);
        // join to first free block if adjacent
        if ((addr) prevFree == (addr) nextFree) {
            // point to new start
            Heap.freeList[0] = h;
            int size = h->size + nextFree->size;
            memset(obj, 0, h->size);
            h->size = size;     
        }
        // else just free chunk
        else {
            memset(obj, 0, (h->size - sizeof(header *)));
            header *prev = Heap.freeList[0];
            header *temp = NULL;
            //shift array to the right
            int i = 1;
            for (; i < Heap.nFree; i++) {
                temp = Heap.freeList[i];
                Heap.freeList[i] = prev;
                prev = temp;    
            }
            Heap.freeList[i] = prev;
            // insert free chunk to empty space in the array
            Heap.freeList[0] = h;
            Heap.nFree++;
        }     
    }
    // object somewhere in the middle
    else {
        int flag = 0;
        // merge right if free chunk on right 
        if (((addr)h + h->size == (addr)nextFree)){
            h->status = FREE;
            Heap.freeList[position] = h;
            int size = h->size + nextFree->size;
            memset(obj, 0, h->size);
            h->size = size;
            flag = 1;
        }
        // merge left if free on left
        if ((addr)prevFree + prevFree->size == (addr) h){
            prevFree->size += h->size;  
            memset(h, 0, h->size);   
            // if both sides adjacent ooga booga, freeList must be adjusted
            if (flag == 1) {
                int i = position;
                for (; i < Heap.nFree - 2; i++) {
                    Heap.freeList[i] = Heap.freeList[i + 1];
                }
                Heap.freeList[i] = NULL;
                Heap.nFree--;
            }
            flag = 1;        
        }
        // if no adjacent, just free chunk
        if (flag == 0) {
            h->status = FREE;
            memset(obj, 0, (h->size - sizeof(header *)));
            header *prev = Heap.freeList[position];
            header *temp = NULL;
            int i = position + 1;
            //shift array to the right from position
            for (; i < Heap.nFree; i++) {
                temp = Heap.freeList[i];
                Heap.freeList[i] = prev;
                prev = temp;
            }
            Heap.freeList[i] = prev;
            // insert free chunk to empty space in the array
            Heap.freeList[position] = h;
            Heap.nFree++;
        }
    }
}

/** Return the first address beyond the range of the heap. */
static addr heapMaxAddr (void)
{
	return (addr) Heap.heapMem + Heap.heapSize;
}

/** Convert a pointer to an offset in the heap. */
int heapOffset (void *obj)
{
	addr objAddr = (addr) obj;
	addr heapMin = (addr) Heap.heapMem;
	addr heapMax =        heapMaxAddr ();
	if (obj == NULL || !(heapMin <= objAddr && objAddr < heapMax))
		return -1;
	else
		return (int) (objAddr - heapMin);
}

/** Dump the contents of the heap (for testing/debugging). */
void dumpHeap (void)
{
	int onRow = 0;

	// We iterate over the heap, chunk by chunk; we assume that the
	// first chunk is at the first location in the heap, and move along
	// by the size the chunk claims to be.
	addr curr = (addr) Heap.heapMem;
	while (curr < heapMaxAddr ()) {
		header *chunk = (header *) curr;

		char stat;
		switch (chunk->status) {
		case FREE:  stat = 'F'; break;
		case ALLOC: stat = 'A'; break;
		default:
			fprintf (
				stderr,
				"myHeap: corrupted heap: chunk status %08x\n",
				chunk->status
			);
			exit (1);
		}

		printf (
			"+%05d (%c,%5d)%c",
			heapOffset ((void *) curr),
			stat, chunk->size,
			(++onRow % 5 == 0) ? '\n' : ' '
		);

		curr += chunk->size;
	}

	if (onRow % 5 > 0)
		printf ("\n");
}
