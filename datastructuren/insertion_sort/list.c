#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

struct node {
    int data;
    struct node* next;
    struct node* prev;
};

struct list {
    struct node* head;
    struct node* tail;
    size_t length;
};


struct list *list_init(void) {
    // allocate space for list
    struct list *l = malloc(sizeof(struct list));
    
    if (l == NULL) {
        printf("Out of memory\n");
        return NULL;
    }
    
    l->head = NULL;
    l->tail = NULL;
    l->length = 0;
    return l;
}

struct node *list_new_node(int num) {
    struct node *n = malloc(sizeof(struct node)); // space for one node
    // if succeeds then n  = address 

    if (n == NULL) {
        printf("Out of memory\n");
        return NULL;
    }

    n->data = num;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

// check if no list -> return null 
// otherwise return * first node 
struct node *list_head(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }

    return l->head;
}

// if have no node -> return null
// else return pointer to next node
struct node *list_next(const struct node *n) {
    if (n == NULL) {
        return NULL;
    }

    return n->next;
}

// node n at the front of l 
// if success return 0 else 1
// new node is first; 
int list_add_front(struct list *l, struct node *n) {
    if (l == NULL || n == NULL) {
        return 1;
    }

    n->prev = NULL; // node at the beginning
    n->next = l->head; // next points to old head

    if (l->head != NULL) { 
        l->head->prev = n; // old head's prev points to new node
    } else {
        l->tail = n; // if list empty-> tail points to new node
    }

    l->head = n; // head points to new node
    l->length++; // increase length (new node added)

    return 0;
}

struct node *list_tail(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }

    return l->tail;
}

// for moving backwards in the list
// if no list or no node -> return null
// if n is first node -> return null
// else return pointer to previous node
struct node *list_prev(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL) {
        return NULL;
    }

    return n->prev;
}

// append node n at the back of l
// if success return 0 else 1
int list_add_back(struct list *l, struct node *n) {
    // check if l and n exist
    if (l == NULL || n == NULL) {
        return 1;
    } 

    n->next = NULL; // new node at the end
    n->prev = l->tail; // prev points to old tail

    if (l->tail != NULL) {
        l->tail->next = n; // old tail's next points to new node
    } else {
        l->head = n; // if list empty-> head points to new node
    }

    l->tail = n; // tail points to new node
    l->length++; // increase length (new node added)

    return 0;
}

int list_node_get_value(const struct node *n) {
    // check if exists 
    if (n == NULL) { // gno node to read from  
        return 0;
    }
    // get the data  
    return n->data;
}

// change value of node 
// if success return 0 else 1
int list_node_set_value(struct node *n, int value) {
    // check if n exists
    if (n == NULL) {
        return 1;
    }

    n->data = value;
    return 0;
}

// unlink node n from list l
// after: l - no pointers to n and n - no pointers to l
// if success return 0 else 1
// unlink node n from list l
// after: l - no pointers to n and n - no pointers to l
// if success return 0 else 1
int list_unlink_node(struct list *l, struct node *n) {

    if (l == NULL || n == NULL) {
        return 1; // error
    }

    // n is head
    if (n == l->head) {
        l->head = n->next;         // move head forward
        if (l->head != NULL) {
            l->head->prev = NULL;  // new head has no prev
        } else {
            l->tail = NULL;        // list became empty
        }
    }

    // n is tail
    else if (n == l->tail) {
        l->tail = n->prev;         // move tail backward
        if (l->tail != NULL) {
            l->tail->next = NULL;  // new tail has no next
        } else {
            l->head = NULL;        // list became empty
        }
    }

    // n is in middle
    else {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }

    // disconnect n fully
    n->next = NULL;
    n->prev = NULL;

    l->length--; // decrease length

    return 0; // DO NOT FREE HERE
}

// clean up entire list l
// if success return 0 else 1
// free all nodes and the list itself
// set head and tail to NULL
// length to 0
// free all nodes
int list_cleanup(struct list *l) {
    // check if l exists
    if (l == NULL) {
        return 1;
    }

    struct node* going = l->head; 
    struct node* next_node; 

    while (going != NULL) {
        next_node = going->next; // store next node
        free(going); // free current node
        going = next_node; // move to next node
    }

    l->head = NULL;
    l->tail = NULL;
    l->length = 0;

    free(l); // free list structure itself

    return 0;
}

// check if n present in l 
// if present return 1
// if not present return 0
// if error return -1
int list_node_present(const struct list *l, const struct node *n) {
    // check if l and n exist
    if (l == NULL || n == NULL) {
        return 0;
    }

    struct node* going = l->head;
    while (going != NULL) {
        if (going == n) {
            return 1;
        }

        going = going->next;
    }

    return 0;
}

// insert n after m in l 
// if m not in l or n already in l -> fail
// if success return 0 else 1
int list_insert_after(struct list *l, struct node *n, struct node *m) {
    // check if l, n, m exist
    if (l == NULL || n == NULL || m == NULL) {
        return 1;
    }

    // check if m in l
    if (list_node_present(l, m) == 0) {
        return 1;
    } 

    // check if n already in l
    if (list_node_present(l, n) == 1) {
        return 1; 
    }

    n->prev = m; // n's prev * to m
    n->next = m->next; // n's next * to m's next
    if (m->next != NULL) {
        m->next->prev = n; // m's next prev * to n 
    } else {
        l-> tail = n; // if m tail -> update tail to n 
    } 

    m->next = n; // m's next * to n
    l->length++; 

    return 0;
}

// insert n before m in l
// if m not in l or n already in l -> fail
// if success return 0 else 1
int list_insert_before(struct list *l, struct node *n, struct node *m) {
    // check if l, n, m exist
    if ( l == NULL || n == NULL || m == NULL) {
        return 1; 
    }

    // check if m in l
    if (list_node_present(l, m) == 0) {
        return 1;
    }

    // check if n already in l
    if (list_node_present(l, n) == 1) {
        return 1;
    }

    n->next = m;; // n's next * to m
    n->prev = m->prev; // n's prev * to m's prev
    if( m-> prev != NULL) {
        m->prev->next = n; // m's prev next * to n
    } else {
        l-> head = n; // if m head -> update head to n
    }

    m->prev = n; // m's prev * to n
    l->length++;

    return 0;
}

// return length of l
// if l is NULL return 0
size_t list_length(const struct list *l) {
    // check if l exists
    if (l == NULL) {
        return 0;
    }

    return l->length; 
}

// get ith node in l
// if i >= length return NULL
// else return * to ith node
struct node *list_get_ith(const struct list *l, size_t i) {
    // check if l exists and i in range
    if (l == NULL || i >= l->length) {
        return NULL; 
    }

    struct node* going = l->head;
    size_t index = 0;
    while (going != NULL) {
        if (index == i) {
            return going;
        }
        going = going->next;
        index++;
    }

    return NULL;
}

// cut list l after node n
// return * to new list
// if n is NULL return entire list l and set l to empty
// if n is last node return empty list and l unchanged
struct list *list_cut_after(struct list *l, struct node *n) {
    // check if l exists
    if (l == NULL) {
        return NULL;
    }

    struct list* new_list = list_init();
    if (new_list == NULL) {
        return NULL;
    }

    // if n is NULL -> return entire list l 
    if ( n == NULL) {
        new_list->head = l->head;
        new_list->tail = l->tail; 
        // break the link between l and new list 
        new_list->length = l->length; 
        l->head = NULL; 
        l->tail = NULL; 
        l->length = 0;  
        return new_list;

        struct node* going = n->next;
        // if n is last node -> return empty list
        while (going != NULL) {
            struct node* next_node = going->next;
            list_unlink_node(l, going);
            list_add_back(new_list, going);
            going = next_node;
        }

        //adjest lengths
        l->length -= new_list->length;
        return new_list;

    }

    return NULL;
}
