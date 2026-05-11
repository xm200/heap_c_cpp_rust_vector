#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


/* 
 * 				Preamble.
 * 	At 10.05 i have been listening heap class for 2 hours.
 * So i decided to implement heap structure which dynamicly
 * allocates memory. As well-known example i have decided to use
 * vector of integers.
*/
struct vector {
	size_t size;
	size_t capacity;
	double * elems;
};


/* function which is creating empty vector */
struct vector* alloc_vector() {
	struct vector* vec = malloc(sizeof(struct vector));
	return vec;
}


/*
 * Function need to prevent DF and UAF, thats why it must be used only like:
 * struct vector* vec = realloc_vector(old_vec)
 * and we **must** to lose old ptr value
 *
 * The best way would be to call some internal realloc tricks. Need to readthedocs
*/
struct vector* realloc_vector(struct vector* old_vec) {
	// fully implement in future
	struct vector * new_vec = malloc(sizeof(struct vector));

	// allocate new vector
	new_vec->size = old_vec->size;
	new_vec->capacity = old_vec->capacity << 1;
	new_vec->elems = calloc(new_vec->capacity, sizeof(double));
	
	// allocation is going bad!
	if (!new_vec->elems) {
		// memory is out, the good way here to call global garbage collector
		// but i would not implement this for a while.
		// Let`s pretend that now it exists.
		free(new_vec);
		return NULL;
	}

	// move data from old_vec to new_vec
	memmove(new_vec->elems, old_vec->elems, old_vec->size * sizeof(double));
	
	// free old vector
	free(old_vec->elems);
	free(old_vec);

	return new_vec;
}


/*
 * Function to store elements in given vector
*/
bool put_elem(struct vector** vec, double val) {
	// we will put elements one-by-one
	if ((*vec)->size == (*vec)->capacity) { 
		struct vector* tmp = realloc_vector(*vec);
		if (!tmp) return false;
		*vec = tmp;
	}
	if (!(*vec)) return false;

	(*vec)->elems[(*vec)->size++] = val;
	return true;
}


/*
 * C-style vector cleaning
*/
void kill_vector(struct vector** vec) {
	// guard against null-vector
	if (!vec || !(*vec)) return;
	// free in c style: elems, pointer. Then null the pointer
	free((*vec)->elems);
	free((*vec));
	*vec = 0;
}

void print_menu() {
	printf("a. Allocate vector\n");
	printf("e. Put element in vector\n");
	printf("p. Print vector\n");
	printf("q. Quit\n");
}

int main() {
	char choise;
	bool is_allocated;
	struct vector *vec;
	
	print_menu();
	while (true) {
		printf("> ");
		scanf(" %c", &choise);
		// printf("\n");
		switch (choise) {
			case 'a': {
				if (!is_allocated) {
					vec = alloc_vector();
					is_allocated = true;
					printf("Vector allocated! addr: %p\n", vec);
				} else {
					printf("Vector is already allocated!\n");
				}
				break;
			} case 'e': {
				if (is_allocated) {
					double val;
					scanf("%lf", &val);
					bool success = put_elem(&vec, val);
					if (success) {
						printf("Element stored!\n");
					} else {
						printf("Element is not stored! Erreur! Panic!\n");
						goto end;
					}
				} else {
					printf("Vector is not allocated!\n");
				}
				break;
			} case 'p': {
				if (is_allocated) {
					for (size_t i = 0; i < vec->size; i++) {
						printf("[idx]: %ld\t[elem]: %lf\n", i, vec->elems[i]);
					}
				}
				break;
			} case 'q': {
				printf("Goodbye!\n");
				goto end;
			}
		}
		print_menu();
	}

end:
	kill_vector(&vec);
	return 0;
}

