#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define BUF_SIZE 1024

char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until you implement the
       extra command-line arguments. */

    /* Set to 1 if -d is specified, 0 otherwise. */
    int descending_order;
    // Set to 1 if -c is specified, 0 otherwise.
    int combine;
    // Set to 1 if -o is specified, 0 otherwise.
    int remove_odd;
    /* Set to 1 if -z is specified, 0 otherwise. */
    int zip_alternating;
};

/* function: parse_options
   parse command-line options
   returns 0 on success, 1 on failure
*/
int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

/* function: comes_first
   returns 1 if a must come before b in the sorted list
*/
static int comes_first(int a, int b, int descending) {
    if (descending == 1) {
        if (a > b) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (a < b) {
            return 1;
        } else {
            return 0;
        }
    }
}

/* function: add_number
   insert num at its sorted place in l
   returns 0 on success, 1 on failure
*/
static int add_number(struct list *l, int num, int descending) {
    struct node *n = list_new_node(num);
    if (!n) {
        return 1;
    }

    struct node *tail = list_tail(l);
    if (!tail || !comes_first(num, list_node_get_value(tail), descending)) {
        return list_add_back(l, n);
    }

    struct node *current = list_head(l);
    while (!comes_first(num, list_node_get_value(current), descending)) {
        current = list_next(current);
    }
    return list_insert_before(l, n, current);
}

/* function: take_front
   unlink and return the first node of l
*/
static struct node *take_front(struct list *l) {
    struct node *n = list_head(l);
    if (n) {
        list_unlink_node(l, n);
    }
    return n;
}

/* function: combine_pairs
   -c: add the values pair-wise
*/
static void combine_pairs(struct list *l) {
    size_t length = list_length(l);
    for (size_t i = 0; i < length; i += 2) {
        struct node *first = take_front(l);
        if (i + 1 < length) {
            struct node *second = take_front(l);
            int sum = list_node_get_value(first) + list_node_get_value(second);
            list_node_set_value(first, sum);
            list_free_node(second);
        }
        list_add_back(l, first);
    }
}

/* function: remove_odd
   -o: remove all odd values
*/
static void remove_odd(struct list *l) {
    size_t length = list_length(l);
    for (size_t i = 0; i < length; i++) {
        struct node *n = take_front(l);
        if (list_node_get_value(n) % 2 != 0) {
            list_free_node(n);
        } else {
            list_add_back(l, n);
        }
    }
}

/* function: zip_halves
   -z: cut l in two halves and zip them together
   returns 0 on success, 1 on failure
*/
static int zip_halves(struct list *l) {
    size_t length = list_length(l);
    if (length < 2) {
        return 0;
    }

    size_t half = (length + 1) / 2;
    struct list *second = list_cut_after(l, list_get_ith(l, half - 1));
    if (!second) {
        return 1;
    }

    for (size_t i = 0; i < half; i++) {
        list_add_back(l, take_front(l));
        struct node *other = take_front(second);
        if (other) {
            list_add_back(l, other);
        }
    }

    list_cleanup(second);
    return 0;
}

/* function: print_list
   print every value on its own line
*/
static void print_list(const struct list *l) {
    for (struct node *n = list_head(l); n; n = list_next(n)) {
        printf("%d\n", list_node_get_value(n));
    }
}

/* function: main
   main program logic
*/
int main(int argc, char *argv[]) {
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }

    struct list *l = list_init();
    if (!l) {
        fprintf(stderr, "out of memory\n");
        return 1;
    }

    int num = 0;
    bool reading = false;
    int error = 0;
    while (fgets(buf, BUF_SIZE, stdin)) {
        for (size_t i = 0; buf[i] != '\0'; i++) {
            if (isdigit((unsigned char)buf[i])) {
                num = num * 10 + (buf[i] - '0');
                reading = true;
            } else if (reading) {
                error |= add_number(l, num, cfg.descending_order);
                num = 0;
                reading = false;
            }
        }
    }

    if (reading) {
        error |= add_number(l, num, cfg.descending_order);
    }
    if (!error && cfg.combine) {
        combine_pairs(l);
    }
    if (!error && cfg.remove_odd) {
        remove_odd(l);
    }
    if (!error && cfg.zip_alternating) {
        error = zip_halves(l);
    }

    if (error) {
        fprintf(stderr, "out of memory\n");
        list_cleanup(l);
        return 1;
    }

    print_list(l);
    list_cleanup(l);
    return 0;
}
