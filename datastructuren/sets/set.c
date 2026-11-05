#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "set.h"
#include "tree.h"

struct set {
    struct tree *tree;
    int turbo; 
};

/* Initialize a new set

   turbo: if non-zero, enable turbo mode 

   return: * to new set or NULL on failure
*/
struct set *set_init(int turbo) {
    struct set *new_set = malloc(sizeof(struct set));
    if (new_set == NULL) {
        return NULL;
    }

    new_set->tree = tree_init(turbo);
    new_set->turbo = turbo;

    return new_set;
}

/* Insert num into set

   s: * to set
   num: number to insert

   return: 0 if inserted, 1 if duplicate
*/
int set_insert(struct set *s, int num) {
    if (s == NULL || s->tree == NULL) {
        return 1;
    }

    return tree_insert(s->tree, num);
}

/* Find num in set

   s: * to set
   num: number to find

   return: 1 if found, 0 if not found
*/
int set_find(struct set *s, int num) {
    if (s == NULL || s->tree == NULL) {
        return 0;
    }

    return tree_find(s->tree, num);
}

/* Remove num from set

   s: * to set
   num: number to remove

   return: 0 if removed, 1 if not found
*/
int set_remove(struct set *s, int num) {
    if (s == NULL || s->tree == NULL) {
        return 1;
    }

    return tree_remove(s->tree, num);
}

/* Cleanup set

   s: * to set
*/
void set_cleanup(struct set *s) {
    if (s == NULL) {
        return;
    }

    tree_cleanup(s->tree);
    free(s);
}

/* Print set

   s: * to set
*/
void set_print(const struct set *s) {
    if (s == NULL || s->tree == NULL) {   
        return;
    }

    tree_print(s->tree);
}

/* Verify set implementation (debug function)

   s: * to set

   return: 0 if valid, 1 if invalid
*/
int set_verify(const struct set *s) {
    if (s == NULL || s->tree == NULL) {
        return 1;
    }

    return tree_check(s->tree);            
}
