#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"
#include "hash_func.h"

struct table {
    /* The (simple) array used to index the table */
    struct node **array;
    /* The function used for computing the hash values in this table */
    unsigned long (*hash_func)(const unsigned char *);
    /* Maximum load factor after which the table array should be resized */
    double max_load_factor;
    /* Capacity of the array used to index the table */
    unsigned long capacity;
    /* Current number of elements stored in the table */
    unsigned long load;
};

struct node {
    /* The string of characters that is the key for this node */
    char *key;
    /* A resizing array, containing the all the integer values for this key */
    struct array *value;
    /* Next pointer */
    struct node *next;
};

/* Create a new node with specified key and initial value.
   Return pointer to created node, or NULL on failure. 

   key: string key for the node
   value: initial integer value to store in the node's array

   returns: pointer to created node, or NULL on failure
*/
static struct node *create_node(const char *key, int value) {
    struct node* n = malloc(sizeof(struct node));
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

/* free all nodes in ll 

   n: poingter to head of linked list

   pointer no longer valid after this call
*/
static void free_nodes(struct node* n) {
    while (n != NULL) {
        struct node* next = n->next;
        array_cleanup(n->value);
        free(n->key);
        free(n);
        n = next;
    }
}

/* add node to the ht at the right index 

   t: pointer to hash table
   n: pointer to node to add
   key: key of the node to add

   node is added at the head of ll
*/
static void add_node (struct table* t, struct node* n, const char* key) {
    unsigned long index = 
    t->hash_func((const unsigned char *)key) % t->capacity;

    n->next = t->array[index];  // Link new node to head of chain
    t->array[index] = n;        // Make new node the head
}

/* first double capacity 
   then rehash all elements into new array

   t: pointer to hash table

   return 0 on success, 1 on failure
*/
static int remake_table(struct table *t) {
    unsigned long new_capacity = t->capacity * 2;
    struct node **new_array = calloc(new_capacity, sizeof(struct node *));
    if (new_array == NULL) {
        return 1;
    }

    struct node** old_array = t->array;
    unsigned long old_capacity = t->capacity;

    t->array = new_array;
    t->capacity = new_capacity;

    for (unsigned long i = 0; i < old_capacity; i++) {
        struct node* current = old_array[i];
        while (current != NULL) {
            struct node* next = current->next;
            current->next = NULL;
            add_node(t, current, current->key);
            current = next;
        }
    }

    free(old_array);
    return 0;
}

/* initialize a ht

   capacity: start capacity of the table
   max_load_factor: max load factor before rehash
   hash_func: pointer to hash function to use

   returns: pointer to created table, or NULL on failure
*/
struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {

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

/* insert key-value pair into ht

   t: pointer to hash table
   key: string key to insert
   value: integer value to insert

   return 0 on success, 1 on failure
*/
int table_insert(struct table *t, const char *key, int value) {
    unsigned long index = t->hash_func((const unsigned char *)key) % t->capacity;
    struct node* current = t->array[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (array_append(current->value, value) != 0) {
                return 1;
            }
            return 0;
        }
        current = current->next;
    }

    struct node* new_node = create_node(key, value);
    if (new_node == NULL) {
        return 1;
    }

    add_node(t, new_node, key);
    t->load++;

    double load_factor = (double)t->load / (double)t->capacity;
    if (load_factor > t->max_load_factor) {
        if (remake_table(t) != 0) {
            return 1;
        }
    }

    return 0;
}

/* lookup key in ht

   t: pointer to hash table
   key: string key to lookup

   returns: pointer to array of values for key,
   or NULL if key not found
*/
struct array *table_lookup(const struct table *t, const char *key) {
    unsigned long index = t->hash_func((const unsigned char *)key) % t->capacity;
    struct node* current = t->array[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

/* return current load factor of the table

   t: pointer to hash table

   returns: load factor of the table
*/
double table_load_factor(const struct table *t) {
    if (t->capacity == 0) {
        return -1.0;
    }
    return (double)t->load / (double)t->capacity;
}

/* delete key from ht

   t: pointer to hash table
   key: string key to delete

   return 0 on success, 1 if key not found
*/
int table_delete(struct table *t, const char *key) {
    unsigned long index = t->hash_func((const unsigned char *)key) % t->capacity;
    struct node* current = t->array[index];
    struct node* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                t->array[index] = current->next;
            } else {
                prev->next = current->next;
            }
            array_cleanup(current->value);
            free(current->key);
            free(current);
            t->load--;
            return 0;
        }
        prev = current;
        current = current->next;
    }

    return 1;
}

/* cleanup hash table

   t: pointer to hash table

   pointer no longer valid after this call
*/
void table_cleanup(struct table *t) {
    if (t == NULL) {
        return;
    }

    for (unsigned long i = 0; i < t->capacity; i++) {
        free_nodes(t->array[i]);
    }

    free(t->array);
    free(t);
}
