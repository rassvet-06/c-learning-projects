#include <stdlib.h>
#include "list.h"

struct node {
    int data;
    struct node *next;
    struct node *prev;
};

struct list {
    struct node *head;
    struct node *tail;
    size_t length;
};

struct list *list_init(void) {
    struct list *l = malloc(sizeof(struct list));
    if (!l) {
        return NULL;
    }

    l->head = NULL;
    l->tail = NULL;
    l->length = 0;
    return l;
}

struct node *list_new_node(int num) {
    struct node *n = malloc(sizeof(struct node));
    if (!n) {
        return NULL;
    }

    n->data = num;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

struct node *list_head(const struct list *l) {
    if (!l) {
        return NULL;
    }
    return l->head;
}

struct node *list_next(const struct node *n) {
    if (!n) {
        return NULL;
    }
    return n->next;
}

// node n at the front of l
// if success return 0 else 1
int list_add_front(struct list *l, struct node *n) {
    if (!l || !n) {
        return 1;
    }
    if (n->next || n->prev || l->head == n) {
        return 1;
    }

    n->next = l->head;
    if (l->head) {
        l->head->prev = n;
    } else {
        l->tail = n;
    }

    l->head = n;
    l->length++;
    return 0;
}

struct node *list_tail(const struct list *l) {
    if (!l) {
        return NULL;
    }
    return l->tail;
}

// return node before n
// if n is first node or not in l -> return null
struct node *list_prev(const struct list *l, const struct node *n) {
    if (!l || !n || !n->prev) {
        return NULL;
    }
    if (list_node_present(l, n) != 1) {
        return NULL;
    }
    return n->prev;
}

// append node n at the back of l
// if success return 0 else 1
int list_add_back(struct list *l, struct node *n) {
    if (!l || !n) {
        return 1;
    }
    if (n->next || n->prev || l->head == n) {
        return 1;
    }

    n->prev = l->tail;
    if (l->tail) {
        l->tail->next = n;
    } else {
        l->head = n;
    }

    l->tail = n;
    l->length++;
    return 0;
}

int list_node_get_value(const struct node *n) {
    if (!n) {
        return 0;
    }
    return n->data;
}

int list_node_set_value(struct node *n, int value) {
    if (!n) {
        return 1;
    }
    n->data = value;
    return 0;
}

// unlink node n from list l
// after: l - no pointers to n and n - no pointers to l
// if success return 0 else 1
int list_unlink_node(struct list *l, struct node *n) {
    if (!l || !n) {
        return 1;
    }
    if (list_node_present(l, n) != 1) {
        return 1;
    }

    if (n->prev) {
        n->prev->next = n->next;
    } else {
        l->head = n->next;
    }

    if (n->next) {
        n->next->prev = n->prev;
    } else {
        l->tail = n->prev;
    }

    n->next = NULL;
    n->prev = NULL;
    l->length--;
    return 0;
}

void list_free_node(struct node *n) {
    free(n);
}

// clean up entire list l
// if success return 0 else 1
int list_cleanup(struct list *l) {
    if (!l) {
        return 1;
    }

    struct node *current = l->head;
    while (current) {
        struct node *next = current->next;
        list_free_node(current);
        current = next;
    }

    l->head = NULL;
    l->tail = NULL;
    l->length = 0;
    free(l);
    return 0;
}

// check if n present in l
// if present return 1
// if not present return 0
// if error return -1
int list_node_present(const struct list *l, const struct node *n) {
    if (!l || !n) {
        return -1;
    }

    for (struct node *current = l->head; current; current = current->next) {
        if (current == n) {
            return 1;
        }
    }
    return 0;
}

// insert n after m in l
// if m not in l or n already in l -> fail
// if success return 0 else 1
int list_insert_after(struct list *l, struct node *n, struct node *m) {
    if (!l || !n || !m) {
        return 1;
    }
    if (n->next || n->prev || l->head == n) {
        return 1;
    }
    if (list_node_present(l, m) != 1) {
        return 1;
    }

    n->prev = m;
    n->next = m->next;
    if (m->next) {
        m->next->prev = n;
    } else {
        l->tail = n;
    }

    m->next = n;
    l->length++;
    return 0;
}

// insert n before m in l
// if m not in l or n already in l -> fail
// if success return 0 else 1
int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (!l || !n || !m) {
        return 1;
    }
    if (n->next || n->prev || l->head == n) {
        return 1;
    }
    if (list_node_present(l, m) != 1) {
        return 1;
    }

    n->next = m;
    n->prev = m->prev;
    if (m->prev) {
        m->prev->next = n;
    } else {
        l->head = n;
    }

    m->prev = n;
    l->length++;
    return 0;
}

size_t list_length(const struct list *l) {
    if (!l) {
        return 0;
    }
    if (l->length == 0) {
        return 0;
    }
    return l->length;
}

// get ith node in l
// if i >= length return NULL
struct node *list_get_ith(const struct list *l, size_t i) {
    if (!l || i >= l->length) {
        return NULL;
    }

    struct node *current = l->head;
    for (size_t index = 0; index < i; index++) {
        current = current->next;
    }
    return current;
}

// cut list l after node n
// return * to new list
struct list *list_cut_after(struct list *l, struct node *n) {
    if (!l || !n) {
        return NULL;
    }
    if (list_node_present(l, n) != 1) {
        return NULL;
    }

    struct list *second = list_init();
    if (!second) {
        return NULL;
    }
    if (!n->next) {
        return second;
    }

    size_t moved = 0;
    for (struct node *current = n->next; current; current = current->next) {
        moved++;
    }

    second->head = n->next;
    second->tail = l->tail;
    second->length = moved;
    second->head->prev = NULL;

    n->next = NULL;
    l->tail = n;
    l->length -= moved;
    return second;
}
