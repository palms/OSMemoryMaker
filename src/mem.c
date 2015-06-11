/*
 ============================================================================
 Name        : memAlloc.c
 Author      : Saul Laufer
 Version     :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//not sure if redeclaration here is correct from mem.h
//but i think so, so fuck it
int m_error;

int mem_Init_Calls = 0;

typedef struct tagNode_t {
	int size;
	void *next;
} Node_t;

Node_t *head = NULL;

int Mem_Init(int sizeOfRegion) {
	printf("calling: Mem_Init()\n");

	if (mem_Init_Calls != 0) {
		printf("multiple calls to Mem_Init\n");
		return -1;
	}

	if (sizeOfRegion <= 0) {
		printf("bad args\n");
		m_error = E_BAD_ARGS;
		return -1;
	}

	//sizeOfRegion (in bytes) needs to be
	//evenly divisible by the page size

	//this is the only sketchy part in Mem_Init
	//think it works but haven't tested thoroughly
	int pageSize = getPageSize();

	//if not evenly divisible
	if (sizeOfRegion % pageSize != 0) {
		//since pageSize is already pageSize * 1, counter starts at two
		int counter = 2;
		while (1) {
			//if param is smaller (or equal), it gets rounded up
			if (sizeOfRegion <= pageSize) {
				sizeOfRegion = pageSize;
				break;
			}
			//otherwise, pageSize gets incremented to next page size
			//and test again
			pageSize = pageSize * counter;
			counter++;
		}
	}


	void *ptr = mmap(NULL, sizeOfRegion,
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (ptr == MAP_FAILED) {
		perror("mmap failure");
		return -1;
	}

	//make ptr the head of the list, cast
	head = (Node_t *) ptr;
	head->size = sizeOfRegion;
	head->next = NULL;

	//i think this is the format we should use (use %ll to print)
	printf("mem loc: %llx\n", (long long unsigned) ptr);

	//close device ?
	//close(fd);

	mem_Init_Calls++;
	m_error = 0;
	return 0;
}



void * Mem_Alloc(int size) {


	return NULL;
}



int Mem_Free(void *ptr, int coalesce) {


	return -1;
}



void Mem_Dump() {


	return;
}


