/* Doubly Linked List (data structure used)
    Main program to read integers from stdin,
    insert them into a linked list in sorted order,
    and print the sorted list to stdout.

    command-line options:
    -d: sort in descending order
    -c: combine duplicate values by summing them
    -o: remove odd values
    -z: zip two lists by alternating their elements

    functions: 
    parse_options: parse command-line options
    print: print the linked list
    main: main program logic
*/
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define BUF_SIZE 1024

char buf[BUF_SIZE];

/* function: struct config
   structure to hold command-line options
*/
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
   parse command-line options and populate config structure
   returns 0 on success, 1 on failure

   cfg: pointer to config structure to populate
   argc: argument count
   argv: argument vector
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

/* function: print
   print the linked list to stdout

    l: pointer to the linked list 
*/
void print(struct list* l) {
    if (l == NULL) {
        printf("No list to print.\n");
        return;
    }

    struct node* current = list_head(l);
    while (current != NULL) {
        printf("%d\n", list_node_get_value(current));
        current = list_next(current);
    }

    printf("\n");
}

/* function: main
   main program logic

   argc: argument count
   argv: argument vector

   returns 0 on success, 1 on failure
*/
int main(int argc, char *argv[]) {

    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        printf("Error!\n");
        return 1;
    }

    struct list *l = list_init();
    if (l == NULL) {
        printf("Error!\n");
        return 1;
    }

    // read numbers from stdin
    while (fgets(buf, BUF_SIZE, stdin)) {

        char *ptr = buf;

        // process all numbers in this line
        while (*ptr != '\0') {

            // read next number
            char *endptr;
            int num = strtol(ptr, &endptr, 10);

            // no number found -> stop
            if (endptr == ptr) {
                break;
            }

            // move pointer forward
            ptr = endptr;

            // skip spaces/tabs
            while (*ptr == ' ' || *ptr == '\t') {
                ptr++;
            }

            // if remove odd selected AND number is odd -> skip
            if (cfg.remove_odd && (num % 2 != 0)) {
                continue;
            }

            // new node
            struct node *new_node = list_new_node(num);
            if (new_node == NULL) {
                printf("Error!\n");
                list_cleanup(l);
                return 1;
            }

            // insert depending on order
            if (cfg.descending_order) {
                list_add_front(l, new_node);
            } else {
                list_add_back(l, new_node);
            }
        }
    }

    // always sort ascending before combine 
    for (struct node *i = list_head(l); i != NULL; i = list_next(i)) {
        for (struct node *j = list_next(i); j != NULL; j = list_next(j)) {

            if (list_node_get_value(i) > list_node_get_value(j)) {
                int temp = list_node_get_value(i);
                list_node_set_value(i, list_node_get_value(j));
                list_node_set_value(j, temp);
            }
        }
    }

    // combine adjacent nodes: replace each node with sum of itself + next
    if (cfg.combine) {

        struct node *curr = list_head(l);

        while (curr != NULL && list_next(curr) != NULL) {

            struct node *next = list_next(curr);

            int sum = list_node_get_value(curr) + list_node_get_value(next);
            list_node_set_value(curr, sum);

            list_unlink_node(l, next);
            free(next);

            curr = list_next(curr);
        }
    }

    // no output needed for -z

    // print final list 
    print(l);

    // free list
    list_cleanup(l);
    return 0;
}
