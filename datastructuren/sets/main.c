#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

#define BUF_SIZE 256

/* Cleanup set and buffer, then exit with failure

   buf: * to input buffer
   s: * to set
*/
void cleanup_and_fail(char *buf, struct set *s) {
    if (s) {
        set_cleanup(s);
    }
    free(buf);
    exit(EXIT_FAILURE);
}

/* Main program

   Reads commands from stdin 
   
   + <num> insert num into set
   - <num> remove num from set
   ? <num> find num in set
   p       print the set

   return: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int main(void) {
    char *buf = malloc(BUF_SIZE);
    if (!buf) {
        perror("Could not allocate input buffer");
        return EXIT_FAILURE;
    }
    struct set *s = set_init(0);   /* initialize set with turbo turned off. */

    if (s == NULL) {
        cleanup_and_fail(buf, NULL);
    }

    while (fgets(buf, BUF_SIZE, stdin)) {
        char *endptr;
        char *command;
        char *num_str;
        int num;

        command = strtok(buf, " ");     /* get command: +,-,?,p */
        if (strchr("+-?", *command)) {  /* operation with operand */
            num_str = strtok(NULL, "\n");
            if (!num_str) {
                printf("set operation '%c' requires integer operand\n", *command);
                cleanup_and_fail(buf, s);
            }
            num = (int) strtol(num_str, &endptr, 10);
            if (*endptr != '\0') {
                printf("error converting '%s' to an integer\n", num_str);
                cleanup_and_fail(buf, s);
            }
        }
        switch (*command) {
            case '+':
                if (set_insert(s, num) == 0) {
                    // printf("inserted %d\n", num);
                } else {
                    // printf("duplicate %d\n", num);
                }
                break;
            case '-':
                if (set_remove(s, num) == 0) {
                    // printf("removed %d\n", num);
                } else {
                    // printf("not found %d\n", num);
                }
                break;
            case '?':
                if (set_find(s, num)) {
                    printf("found: %d\n", num);
                } else {
                    printf("not found: %d\n", num);
                }
                break;
            case 'p':
                set_print(s);
                break;
            default:
                printf("unknown command: '%s'\n", command);
                cleanup_and_fail(buf, s);
        }
    }

    if (set_verify(s)) { /* debug function */
        fprintf(stderr, "Set implementation failed verification!\n");
    }
    free(buf);
    set_cleanup(s);
    return EXIT_SUCCESS;
}
