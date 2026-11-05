#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
    int *data;          // dynamically allocated array for queue elements
    size_t front;       // index of the first (oldest) element
    size_t rear;        // index of the next insertion
    size_t size;        // number of elements currently in the queue
    size_t capacity;    // maximum number of elements queue can hold
    size_t push_count;  // total successful pushes
    size_t pop_count;   // total successful pops
    size_t max_size;    // maximum number of elements ever stored
};

/* create a new queue 
 * allocate memory for both struct and internal array
*/
struct queue *queue_init(size_t capacity) {
    struct queue *q = malloc(sizeof(struct queue));  // allocate struct
    if (!q) {
        return NULL;
    }

    q->data = malloc(capacity * sizeof(int));        // allocate integer array
    if (!q->data) {
        free(q);
        return NULL;
    }

    // initialize internal variables
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity = capacity;
    q->push_count = 0;
    q->pop_count = 0;
    q->max_size = 0;
    return q;
}

/* free memory used by queue
 * avoids leaks during BFS
*/
void queue_cleanup(struct queue *q) {
    if (!q) {
        return;
    }

    free(q->data);
    free(q);
}

/* print queue usage statistics
*/
void queue_stats(const struct queue *q) {
    if (!q) {
        return;
    }

    fprintf(stderr, "stats %zu %zu %zu\n",
            q->push_count, q->pop_count, q->max_size);
}

/* adds element at the end of the queue
*/
int queue_push(struct queue *q, int e) {
    if (!q) {
        return 1;
    }

    if (q->size >= q->capacity) {
        size_t new_capacity = q->capacity * 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        int *new_data = malloc(new_capacity * sizeof(int));
        if (!new_data) {
            return 1;
        }
        for (size_t i = 0; i < q->size; i++) {
            new_data[i] = q->data[(q->front + i) % q->capacity];
        }
        free(q->data);
        q->data = new_data;
        q->front = 0;
        q->rear = q->size;
        q->capacity = new_capacity;
    }

    q->data[q->rear] = e;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
    q->push_count++;
    if (q->size > q->max_size) {
        q->max_size = q->size;
    }

    return 0;
}

/* remove the oldest added element
*/
int queue_pop(struct queue *q) {
    if (!q) {
        return -1;
    }

    if (q->size == 0) {
        return -1;  // underflow
    }

    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    q->pop_count++;
    return value;
}

/* get front element without removing it
 * to check what element is at the front of the queue
 */
int queue_peek(const struct queue *q) {
    if (!q) {
        return -1;
    }

    if (q->size == 0) {
        return -1;  // underflow
    }

    return q->data[q->front];
}

/* check if queue is empty
 * to know when to stop BFS
*/
int queue_empty(const struct queue *q) {
    if (!q) {
        return -1;
    }

    return (q->size == 0) ? 1 : 0;
}

/* current number of elements
*/
size_t queue_size(const struct queue *q) {
    if (!q) {
        return 0;
    }

    return q->size;
}
