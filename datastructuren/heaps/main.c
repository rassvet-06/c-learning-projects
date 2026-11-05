#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "prioq.h"

#define BUF_SIZE 1024

/* buffer for reading lines from stdin
*/
static char buf[BUF_SIZE];

/* struct storing program options   
*/
struct config {
    int year;
};

static int parse_options(struct config *cfg, int argc, char *argv[]);

/* struct representing a patient 
*/
typedef struct {
    char *name;
    int birth_year;
} patient_t;

/* comparison function for patient names 

   a: * to first patient
   b: * to second patient

   returns: negative if a < b
            zero if a == b
            positive if a > b
*/
static int compare_patient_name(const void *a, const void *b) {
    return strcmp(((const patient_t *) a)->name, ((const patient_t *) b)->name);
}

/* comparison function for patient birth years 

   a: * to first patient
   b: * to second patient

   returns: negative if a < b
            zero if a == b
            positive if a > b
*/
static int compare_patient_age(const void *a, const void *b) {
    return ((const patient_t *) a)->birth_year - ((const patient_t *) b)->birth_year;
}

/* main program 

   argc: number of command line arguments
   argv: array of command line arguments

   1) parse command line options
   2) initialize priority queue with appropriate comparison function
   3) read patient data from stdin 
        for each line of input
            parse patient name and birth year
            create patient struct and insert into priority queue
        after each iteration, print a dot
   4) after 10 iterations, pop and print all patients 

   returns: EXIT_SUCCESS on success
            EXIT_FAILURE on failure
*/
int main(int argc, char *argv[]) {
    char *token, *name_cpy;
    prioq *queue;
    struct config cfg;

    if (parse_options(&cfg, argc, argv) != 0) {
        return EXIT_FAILURE;
    }

    if (cfg.year) {
        queue = prioq_init(&compare_patient_age);
    } else {
        queue = prioq_init(&compare_patient_name);
    }
    if (queue == NULL) {
        fprintf(stderr, "Failed to initialize priority queue\n");
        return EXIT_FAILURE;
    }

    for (int iterations = 0;;) {

        while (1) {
            char *s = fgets(buf, BUF_SIZE, stdin);
            if (s == NULL) {
                fprintf(stderr, "Unexpected end of file. exiting\n");

                while (prioq_size(queue) > 0) {
                    patient_t *tmp = prioq_pop(queue);
                    free(tmp->name);
                    free(tmp);
                }
                prioq_cleanup(queue, free);

                return EXIT_FAILURE;
            }

            if (strcmp(s, ".\n") == 0 || strcmp(s, ".") == 0) {
                break;
            }

            token = strtok(s, " \t\n");
            if (token == NULL) continue;

            name_cpy = strdup(token);

            token = strtok(NULL, " \t\n");
            if (token == NULL) {
                free(name_cpy);
                continue;
            }

            int year = atoi(token);

            patient_t *p = malloc(sizeof(patient_t));
            p->name = name_cpy;
            p->birth_year = year;

            prioq_insert(queue, p);
        }

        if (prioq_size(queue) > 0) {
            patient_t *p = prioq_pop(queue);
            printf("%s\n", p->name);      
            free(p->name);
            free(p);
        }

        printf(".\n");

        if (++iterations == 10) {

            while (prioq_size(queue) > 0) {
                patient_t *p = prioq_pop(queue);

                printf("%s\n", p->name);

                free(p->name);
                free(p);
            }

            break;
        }
    }

    while (prioq_size(queue) > 0) {
        patient_t *p = prioq_pop(queue);
        free(p->name);
        free(p);
    }

    prioq_cleanup(queue, free);

    return EXIT_SUCCESS;
}

/* parse command line options

   cfg: * to config struct to store options
   argc: number of command line arguments
   argv: array of command line arguments

   returns: 0 on success
            1 on failure
*/
int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "y")) != -1) {
        switch (c) {
        case 'y':
            cfg->year = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}
