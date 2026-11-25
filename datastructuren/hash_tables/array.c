/* resizing array for the line numbers of a word
 * capacity x2 when full
 */
#include <stdlib.h>

#include "array.h"

struct array {
    int *data;
    unsigned long size;
    unsigned long capacity;
};

/* create array with start capacity */
struct array *array_init(unsigned long initial_capacity) {
    struct array *a = malloc(sizeof(struct array));
    if (a == NULL) {
        return NULL;
    }

    if (initial_capacity == 0) {
        initial_capacity = 1;
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

/* free the array */
void array_cleanup(struct array *a) {
    if (a == NULL) {
        return;
    }

    free(a->data);
    free(a);
}

/* get element at index, -1 if invalid */
int array_get(const struct array *a, unsigned long index) {
    if (a == NULL) {
        return -1;
    }
    if (index >= a->size) {
        return -1;
    }
    int elem = a->data[index];
    return elem;
}

/* add element at the end, 0 ok / 1 fail */
int array_append(struct array *a, int elem) {
    if (a == NULL) {
        return 1;
    }

    if (a->size >= a->capacity) {
        unsigned long new_capacity = a->capacity * 2;
        int *new_data = realloc(a->data, new_capacity * sizeof(int));
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

/* number of elements */
unsigned long array_size(const struct array *a) {
    if (a == NULL) {
        return 0;
    }
    return a->size;
}
