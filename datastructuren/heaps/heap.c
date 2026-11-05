// P.S. * represents pointers in comments
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

/* compare function pointer is stored in the heap struct 
    to be used in various heap operations 

    compare: * to a function that compares two items in the heap

    returns: null if fails
            * to a newly created heap 
*/
static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    struct heap *h = malloc(sizeof(struct heap));
    if (h == NULL) {
        return NULL;
    }

    h->array = array_init(8);
    if (h->array == NULL) {
        free(h);
        return NULL;
    }

    h->compare =  compare;

    return h;
}

/* priority queue initialization

   compare: * to a function that compares two items in the queue
*/
prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

/*  return the number of elements in the priority queue
    q: * to the priority queue 

    returns: size of the priority queue
*/
long int prioq_size(const prioq *q) {
    if (q == NULL || q->array == NULL) {
        return -1;
    }

    return array_size(q->array);
}

/* h: * to the heap 
   free_funct: function used to free elemnts inside the heap

   returns: 0 on success or -1 on failure
*/
static int heap_cleanup(struct heap *h, void free_func(void *)) {
    if (h == NULL) {
        return -1;
    }

    array_cleanup(h->array, free_func);
    free(h);
    return 0;   
}

/* frees the heap priority queue

   q: * to the priority queue
   free_func: function used to free elements inside the priority queue

   returns: 0 on success or -1 on failure
*/
int prioq_cleanup(prioq *q, void free_func(void *)) {
    return heap_cleanup(q, free_func);
}

/* insert an element into the heap 

   h: * to the heap
   p: * to the element to be inserted

   1) safety check
   2) add element to the end of the array
   3) bubble up the element to restore heap property
      compare new element with its parent
      if new element has higher priority than parent, swap them
      repeat until the element is in cor. position or it becomes the root
   4) return success

   returns: 0 on success or -1 on failure
*/
static int heap_insert(struct heap *h, void *p) {
    if (h == NULL || h->array == NULL) {
        return -1;
    }

    if (array_append(h->array, p) != 0) {
        return -1;
    }

    long idx = array_size(h->array) - 1;

    while (idx > 0) {
        long parent_idx = (idx - 1) / 2;

        void *item = array_get(h->array, idx);
        void *parent_item = array_get(h->array, parent_idx);

        if (h->compare(item, parent_item) < 0) {

            array_set(h->array, idx, parent_item);
            array_set(h->array, parent_idx, item);

            idx = parent_idx;
        } else {
            break;
        }
    }

    return 0;
}

/* insert an element into the priority queue 

   q: * to the priority queue
   p: * to the element to be inserted

   returns: 0 on success or -1 on failure
*/
int prioq_insert(prioq *q, void *p) {
    return heap_insert(q, p);
}

/* remove and return the highest priority element from the heap

   h: * to the heap

   1) safety check
   2) store the top element to return later
   3) replace the root with the last element in the array
   4) bubble down the new root to restore heap property
        compare new root with its children
        if one of the children has higher priority than the new root, swap them
        repeat until the element is in cor. position or it becomes a leaf

   returns: * to the highest priority element
            or NULL if empty or failure
*/
static void *heap_pop(struct heap *h) {
    if (h == NULL || h->array == NULL || array_size(h->array) == 0) {
        return NULL;
    }

    long n = array_size(h->array);

    void *top = array_get(h->array, 0);
    void *last = array_get(h->array, n - 1);

    array_set(h->array, 0, last);
    array_pop(h->array);

    if (array_size(h->array) == 0) {
        return top;
    }

    long idx = 0;
    long size = array_size(h->array);

    while (1) {
        long left_idx = 2 * idx + 1;
        long right_idx = 2 * idx + 2;
        long smallest_idx = idx;

        if (left_idx < size) {
            void *left_item = array_get(h->array, left_idx);
            void *smallest_item = array_get(h->array, smallest_idx);
            if (h->compare(left_item, smallest_item) < 0) {
                smallest_idx = left_idx;
            }
        }

        if (right_idx < size) {
            void *right_item = array_get(h->array, right_idx);
            void *smallest_item = array_get(h->array, smallest_idx);
            if (h->compare(right_item, smallest_item) < 0) {
                smallest_idx = right_idx;
            }
        }

        if (smallest_idx == idx) {
            break;
        }

        void *cur = array_get(h->array, idx);
        void *smallest_item = array_get(h->array, smallest_idx);

        array_set(h->array, idx, smallest_item);
        array_set(h->array, smallest_idx, cur);

        idx = smallest_idx;
    }

    return top;
}

/* remove and return the highest priority element from the priority queue

   q: * to the priority queue

   returns: * to the highest priority element
            or NULL 
*/
void *prioq_pop(prioq *q) {
    return heap_pop(q);
}
