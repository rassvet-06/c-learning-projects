/* hash table with chaining (linked list per slot)
 * word -> array of line numbers
 * capacity x2 + rehash when load factor too high
 */
#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"

struct table {
    struct node **array;
    unsigned long (*hash_func)(const unsigned char *);
    double max_load_factor;
    unsigned long capacity;
    unsigned long load;
};

struct node {
    char *key;
    struct array *value;
    struct node *next;
};

static unsigned long get_index(const struct table *t, const char *key) {
    return t->hash_func((const unsigned char *)key) % t->capacity;
}

/* new node with a copy of key and first value */
static struct node *create_node(const char *key, int value) {
    struct node *n = malloc(sizeof(struct node));
    if (n == NULL) {
        return NULL;
    }

    n->key = strdup(key);
    if (n->key == NULL) {
        free(n);
        return NULL;
    }

    n->value = array_init(1);
    if (n->value == NULL) {
        free(n->key);
        free(n);
        return NULL;
    }

    if (array_append(n->value, value) != 0) {
        array_cleanup(n->value);
        free(n->key);
        free(n);
        return NULL;
    }

    n->next = NULL;
    return n;
}

static void free_node(struct node *n) {
    array_cleanup(n->value);
    free(n->key);
    free(n);
}

/* add node at head of its slot */
static void add_node(struct table *t, struct node *n) {
    unsigned long index = get_index(t, n->key);
    n->next = t->array[index];
    t->array[index] = n;
}

/* capacity x2 and rehash all nodes */
static int resize_table(struct table *t) {
    unsigned long new_capacity = t->capacity * 2;
    struct node **new_array = calloc(new_capacity, sizeof(struct node *));
    if (new_array == NULL) {
        return 1;
    }

    struct node **old_array = t->array;
    unsigned long old_capacity = t->capacity;
    t->array = new_array;
    t->capacity = new_capacity;

    for (unsigned long i = 0; i < old_capacity; i++) {
        struct node *current = old_array[i];
        while (current != NULL) {
            struct node *next = current->next;
            add_node(t, current);
            current = next;
        }
    }

    free(old_array);
    return 0;
}

/* create empty table, NULL on failure */
struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {
    if (capacity == 0 || hash_func == NULL) {
        return NULL;
    }

    struct table *t = malloc(sizeof(struct table));
    if (t == NULL) {
        return NULL;
    }

    t->array = calloc(capacity, sizeof(struct node *));
    if (t->array == NULL) {
        free(t);
        return NULL;
    }

    t->capacity = capacity;
    t->max_load_factor = max_load_factor;
    t->hash_func = hash_func;
    t->load = 0;
    return t;
}

/* add value to key, 0 ok / 1 fail */
int table_insert(struct table *t, const char *key, int value) {
    if (t == NULL || key == NULL) {
        return 1;
    }

    struct node *current = t->array[get_index(t, key)];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return array_append(current->value, value);
        }
        current = current->next;
    }

    struct node *new_node = create_node(key, value);
    if (new_node == NULL) {
        return 1;
    }

    add_node(t, new_node);
    t->load++;

    if (table_load_factor(t) > t->max_load_factor) {
        if (resize_table(t) != 0) {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

/* array of values of key, NULL if not found */
struct array *table_lookup(const struct table *t, const char *key) {
    if (t == NULL || key == NULL) {
        return NULL;
    }

    struct node *current = t->array[get_index(t, key)];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

/* load / capacity, -1.0 on error */
double table_load_factor(const struct table *t) {
    if (t == NULL) {
        return -1.0;
    }
    return (double)t->load / (double)t->capacity;
}

/* remove key, 0 ok / 1 not found / -1 error */
int table_delete(struct table *t, const char *key) {
    if (t == NULL || key == NULL) {
        return -1;
    }

    unsigned long index = get_index(t, key);
    struct node *current = t->array[index];
    struct node *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                t->array[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free_node(current);
            t->load--;
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 1;
}

/* free table and all nodes */
void table_cleanup(struct table *t) {
    if (t == NULL) {
        return;
    }

    for (unsigned long i = 0; i < t->capacity; i++) {
        struct node *current = t->array[i];
        while (current != NULL) {
            struct node *next = current->next;
            free_node(current);
            current = next;
        }
    }

    free(t->array);
    free(t);
}
