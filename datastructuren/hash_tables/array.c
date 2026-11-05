#include <stdlib.h>

#include "array.h"

struct array {
    int* data;
    unsigned long size;
    unsigned long capacity;
};

/* create new dynamic array 

   initial_capacity: initial capacity of the array

   returns: pointer to the created array, 
   or NULL on failure
*/
struct array *array_init(unsigned long initial_capacity) {
    struct array *a = malloc(sizeof(struct array));
    if (a == NULL) {
        return NULL;
    }

    a->data = malloc(initial_capacity * sizeof(int));
    if (a->data == NULL) {
        free(a);
        return NULL;
    }

    a->size = 0;
    a->capacity = initial_capacity;

    return a;
}

/* free memory allocated for the array

   a: pointer to the array to cleanup  
   
   pointer no longer valid after this call
*/
void array_cleanup(struct array *a) {
    if (a == NULL) {
        return;
    }

    free(a->data);
    free(a);
}

/* get element at index position in the array

   a: pointer to the array
   index: index position to get element from

   returns: element at index position,
   or -1 if index is invalid
*/
int array_get(const struct array *a, unsigned long index) {
    if (index >= a->size) {
        return -1;
    }
    return a->data[index];
}

/* append element to the end of the array

   a: pointer to the array
   elem: element to append

   returns: 0 if successful, 1 otherwise
*/
int array_append(struct array *a, int elem) {
    unsigned long new_capacity;
    if (a->size >= a->capacity) {
        new_capacity = a->capacity * 2;

        int* new_data = realloc(a->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return 1;
        }
        
        a->data = new_data;
        a->capacity = new_capacity;
    }

    a->data[a->size] = elem;
    a->size++;

    return 0;
}

/* get current number of elements in the array

   a: pointer to the array

   returns: number of elements in the array
*/
unsigned long array_size(const struct array *a) {
    return a->size;
}
