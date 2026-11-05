#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "array.h"
#include "hash_func.h"
#include "hash_table.h"

#define LINE_LENGTH 4096

#define TABLE_START_SIZE 256
#define MAX_LOAD_FACTOR 0.6
#define HASH_FUNCTION hash_too_simple

#define START_TESTS 2
#define MAX_TESTS 2
#define HASH_TESTS 1

/* Replace every non-ascii char with a space and lowercase every char

   line: string to clean up

   modifies the string
*/
static void cleanup_string(char *line) {
    for (char *c = line; *c != '\0'; c++) {
        *c = (char) tolower(*c);
        if (!isalpha(*c)) {
            *c = ' ';
        }
    }
}

/* Return a pointer to a heap allocated string with all the characters we
 * use as word delimiters. Return NULL on failure. */
static char *calc_delim(void) {
    const size_t ascii_table_size = 128;
    char *res = malloc(sizeof(char) * ascii_table_size);
    if (!res) {
        return NULL;
    }

    int res_index = 0;
    for (unsigned char c = 1; c < ascii_table_size; c++) {
        if (!isalpha(c)) {   /* if it's not an alpha it's a delimiter char. */
            res[res_index++] = (char) c;
        }
    }
    res[res_index] = '\0';
    return res;
}

/* Creates a hash table with a word index for the specified file and
 * parameters

   filename: name of the file to read words from
   start_size: initial capacity of the hash table
   max_load: maximum load factor of the hash table
   hash_func: pointer to hash function to use

   returns: pointer to created hash table, or NULL on failure
 */
static struct table *create_from_file(char *filename,
                               unsigned long start_size,
                               double max_load,
                               unsigned long (*hash_func)(const unsigned char *)) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return NULL;

    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        fclose(fp);
        return NULL;
    }

    struct table *hash_table = table_init(start_size, max_load, hash_func);
    if (hash_table == NULL) {
        fclose(fp);
        free(line);
        return NULL;
    }

    unsigned long line_number = 0;  // Track line numbers
    while (fgets(line, LINE_LENGTH, fp)) {
        line_number++;              // Increment BEFORE processing
        cleanup_string(line);

        char *delim = calc_delim();
        if (delim == NULL) {
            table_cleanup(hash_table);
            fclose(fp);
            free(line);
            return NULL;
        }

        char *word = strtok(line, delim);
        while (word != NULL) {
            if (table_insert(hash_table, word, (int)line_number) != 0) {  // Store line number
                free(delim);
                table_cleanup(hash_table);
                fclose(fp);
                free(line);
                return NULL;
            }

            word = strtok(NULL, delim);
        }
        free(delim);
    }
    fclose(fp);
    free(line);

    return hash_table;
}

/* Reads words from stdin and prints line lookup results per word.

   hash_table: pointer to hash table to lookup words in

   returns: 0 on success, 1 on failure
 */
static int stdin_lookup(struct table *hash_table) {
    char *line = malloc(LINE_LENGTH * sizeof(char));
    if (!line) {
        return 1;
    }

    while (fgets(line, LINE_LENGTH, stdin)) {
        cleanup_string(line);

        char *word = strtok(line, " \n");
        while (word != NULL) {
            struct array *result = table_lookup(hash_table, word);
            if (result == NULL) {
                printf("Word '%s' not found in table.\n", word);
            } else {
                printf("%s\n", word);  // Print word first
                for (unsigned long i = 0; i < array_size(result); i++) {
                    printf("* %d\n", array_get(result, i));  // Print line numbers
                }
            }
            word = strtok(NULL, " \n");
            printf("\n");
        }
    }
    free(line);
    return 0;
}

/* Test timed construction of hash tables with different parameters

   filename: name of the file to read words from
*/
static void timed_construction(char *filename) {
    unsigned long start_sizes[START_TESTS] = { 2, 65536 };
    double max_loads[MAX_TESTS] = { 0.2, 1.0 };
    unsigned long (*hash_funcs[HASH_TESTS])(const unsigned char *) = { hash_too_simple };

    for (int i = 0; i < START_TESTS; i++) {
        for (int j = 0; j < MAX_TESTS; j++) {
            for (int k = 0; k < HASH_TESTS; k++) {
                clock_t start = clock();
                struct table *hash_table =
                create_from_file(filename, start_sizes[i], max_loads[j], hash_funcs[k]);
                clock_t end = clock();

                printf("Start: %ld\tMax: %.1f\tHash: %d\t -> Time: %ld "
                       "microsecs\n",
                       start_sizes[i], max_loads[j], k, end - start);
                table_cleanup(hash_table);
            }
        }
    }
}

/* Main function

   argc: number of command line arguments
   argv: array of command line argument strings

   returns: EXIT_SUCCESS on success, EXIT_FAILURE on failure
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s text_file [-t]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && !strcmp(argv[2], "-t")) {
        timed_construction(argv[1]);
    } else {
        struct table *hash_table =
        create_from_file(argv[1], TABLE_START_SIZE, MAX_LOAD_FACTOR, HASH_FUNCTION);
        if (hash_table == NULL) {
            printf("An error occured creating the hash table, exiting..\n");
            return EXIT_FAILURE;
        }
        if (stdin_lookup(hash_table) != 0) {
            table_cleanup(hash_table);
            return EXIT_FAILURE;
        }
        table_cleanup(hash_table);
    }

    return EXIT_SUCCESS;
}
