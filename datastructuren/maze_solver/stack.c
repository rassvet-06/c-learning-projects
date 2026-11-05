#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack {
    int *data;          // array for stack elements
    size_t top;         // index of next free position
    size_t capacity;    // total allocated elements
    size_t push_count;  // number of pushes
    size_t pop_count;   // number of pops
    size_t max_size;    // maximum number of elements stored
};

/* initialize stack to prepare for use
 * so DFS can use to see what cells to visit next
*/
struct stack *stack_init(size_t capacity) {
    struct stack *s = malloc(sizeof(struct stack));
    if (!s) {
        return NULL;
    }

    s->data = malloc(capacity * sizeof(int));
    if (!s->data) {
        free(s);
        return NULL;
    }

    s->top = 0;
    s->capacity = capacity;
    s->push_count = 0;
    s->pop_count = 0;
    s->max_size = 0;
    return s;
}

/* free memory used by stack
 * to avoid memory leaks
*/
void stack_cleanup(struct stack *s) {
    if (!s) {
        return;
    }

    free(s->data);
    free(s);
}

/* print stack usage statistics
 * to ensure logic works
*/
void stack_stats(const struct stack *s) {
    if (!s) {
        return;
    }
    fprintf(stderr, "stats %zu %zu %zu\n",
            s->push_count, s->pop_count, s->max_size);
}

int stack_push(struct stack *s, int c) {
    if (!s) {
        return 1;
    }

    if (s->top >= s->capacity) {
        size_t new_capacity = s->capacity * 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        int *new_data = realloc(s->data, new_capacity * sizeof(int));
        if (!new_data) {
            return 1;
        }
        s->data = new_data;
        s->capacity = new_capacity;
    }

    s->data[s->top++] = c;
    s->push_count++;
    if (s->top > s->max_size) {
        s->max_size = s->top;
    }

    return 0;
}

int stack_pop(struct stack *s) {
    if (!s) {
        return -1;
    }

    if (s->top == 0) {
        return -1;  // underflow
    }

    s->pop_count++;
    return s->data[--s->top];
}

/* get top element without removing it
 * to check what the next maze cell will be
*/
int stack_peek(const struct stack *s) {
    if (!s) {
        return -1;
    }

    if (s->top == 0) {
        return -1;
    }

    return s->data[s->top - 1];
}

/* check if stack is empty
 * to know when to stop DFS
*/
int stack_empty(const struct stack *s) {
    if (!s) {
        return -1;
    }

    return (s->top == 0) ? 1 : 0;
}

/* current number of elements
*/
size_t stack_size(const struct stack *s) {
    if (!s) {
        return 0;
    }
    
    return s->top;
}
