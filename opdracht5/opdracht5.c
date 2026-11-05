/*
*
*  Simple DNA Matcher — cleaned rewrite
*  function levenshtein: computes the Levenshtein distance between two
*  strings
*  function cmd_help: list of commands
*  function cmd_list: print database
*  function cmd_add: add to database
*  (no duplicates, case insensitive, only a-z)
*  function cmd_compare: compare two strings (print matrix)
*  function cmd_delete: remove from database
*  function cmd_read: read from file and add to database
*  function cmd_retrieve: find in database (print top 3 matches)
*  function cmd_size: print number of items in database
*  used for info: https://www.geeksforgeeks.org/competitive-programming/dynamic-programming/
*  https://www.geeksforgeeks.org/dsa/introduction-to-dynamic-programming-data-structures-and-algorithm-tutorials/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_DB_SIZE 100
#define MAX_INPUT_SIZE 1024

static char *DATABASE[MAX_DB_SIZE];
static int DB_SIZE = 0;

/* Function levenshtein
*  Computes the Levenshtein distance between s1 and s2
*  If debug is true, print the matrix
*  Uses O(m) space instead of O(n*m)
*  s1: first string
*  s2: second string
*  debug: print matrix if true
*  returns: distance
*/
static int levenshtein(const char *s1, const char *s2, bool debug) {
    int n = (int)strlen(s1);
    int m = (int)strlen(s2);
    if (n == 0) return m;
    if (m == 0) return n;

    int *prev = malloc((size_t)(m + 1) * sizeof *prev);
    int *curr = malloc((size_t)(m + 1) * sizeof *curr);
    if (!prev || !curr) {
        perror("malloc");
        exit(EXIT_FAILURE);     
    }

    for (int j = 0; j <= m; ++j) prev[j] = j;

    // Print initial row (row 0)
    if (debug) {
        for (int j = 0; j <= m; j++) {
            printf("%d ", prev[j]);
        }
        printf("\n");
    }
    for (int i = 1; i <= n; ++i) {
        curr[0] = i;
        for (int j = 1; j <= m; ++j) {
            int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
            int del = prev[j] + 1;
            int ins = curr[j-1] + 1;
            int rep = prev[j-1] + cost;
            int best = del < ins ? del : ins;
            if (rep < best) best = rep;
            curr[j] = best;
        }

        if (debug) {
            for (int j = 0; j <= m; j++) {
                printf("%d ", curr[j]);
            }
            printf("\n");
        }

        int *tmp = prev; prev = curr; curr = tmp;
    }

    int result = prev[m];
    free(prev);
    free(curr);
    return result;
}

/* Function cmd_help: list of commands
*  prints list of commands 
*/
static void cmd_help(void) {
    printf("LIST OF COMMANDS\n");
    printf("  add       add to database\n");
    printf("  compare   compare two strings\n");
    printf("  help      print this text\n");
    printf("  list      print database\n");
    printf("  read      read from file and add to database\n");
    printf("  remove    remove from database\n");
    printf("  retrieve  find in database\n");
    printf("  size      print number of items in database\n");
    printf("  quit      stop\n");
}

/* Function cmd_list: print database
*  prints database contents
*/
static void cmd_list(void) {
    if (DB_SIZE == 0) {
        printf("Database is empty\n");
        return;
    }

    for (int i = 0; i < DB_SIZE; ++i) {
        printf("%s\n", DATABASE[i]);
    }
}

/* Function find_index: find string in database
*  s: string to find
*  returns: index or -1 if not found
*/
static int find_index(const char *s) {
    for (int i = 0; i < DB_SIZE; ++i) {
        if (strcmp(DATABASE[i], s) == 0) return i;
    }
    return -1;
}

/* Function cmd_add: add to database
*  s: string to add
*/
static void cmd_add(const char *s) {
    char kopie_input[MAX_INPUT_SIZE];
    const char *lees = s;
    char *schrijf = kopie_input;
// conversion lowercase
    while (*lees) {
        unsigned char uc = (unsigned char)*lees;
        if (isalpha(uc)) {
            *schrijf++ = (char)tolower(uc);
        }
        lees++;
    }
    *schrijf = '\0';

    if (find_index(kopie_input) >= 0) {
        printf("\"%s\" not added; already in database\n", kopie_input);
        return;
    }

    if (DB_SIZE >= MAX_DB_SIZE) {
        printf("\"%s\" not added; database is full\n", kopie_input);
        return;
    }

    char *copy = malloc(strlen(kopie_input) + 1);
    if (!copy) { perror("malloc"); exit(EXIT_FAILURE); }
        strcpy(copy, kopie_input);
        DATABASE[DB_SIZE++] = copy;

    printf("\"%s\" added to database\n", kopie_input);
}

/* Function cmd_compare: compare two strings (print matrix)
*  s1: first string
*  s2: second string
*  prints distance
*/
static void cmd_compare (const char *s1, const char *s2) {
    char str1[MAX_INPUT_SIZE];
    char str2[MAX_INPUT_SIZE];

    // convert to lowercase and remove non-alpha
    const char *lees = s1;
    char *schrijf = str1;
    while (*lees) {
        unsigned char uc = (unsigned char)*lees;
        if (isalpha(uc)) {
            *schrijf++ = (char)tolower(uc);
        }
        lees++;
    }
    *schrijf = '\0';

    lees = s2;
    schrijf = str2;
    while (*lees) {
        unsigned char uc = (unsigned char)*lees;
        if (isalpha(uc)) {
            *schrijf++ = (char)tolower(uc);
        }
        lees++;
    }
    *schrijf = '\0';

    int dist = levenshtein(str1, str2, true);
    printf("Difference = %d\n", dist);
}

/* Function cmd_delete: remove from database
*  s: string to remove
*/
static void cmd_delete(const char *s) {
    int idx = find_index(s);
    if (idx < 0) {
        printf("\"%s\" not removed; not in database\n", s);
        return;
    }

    free(DATABASE[idx]);
    for (int i = idx; i < DB_SIZE - 1; ++i) DATABASE[i] = DATABASE[i+1];
    DATABASE[--DB_SIZE] = NULL;
    printf("\"%s\" removed from database\n", s);
}

/* Function cmd_read: read from file and add to database
*  filename: name of file
*/
static void cmd_read(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen"); return; }
    char word[MAX_INPUT_SIZE];
    while (fscanf(f, "%1023s", word) == 1) {
        cmd_add(word);
    }
    fclose(f);
}

/* Function cmd_retrieve: find in database (print top 3 matches)
*  s: string to find
*/
static void cmd_retrieve(const char *s) {
    if (DB_SIZE == 0) { 
        printf("No match found; database is empty \n"); 
        return; 
    }

    if (find_index(s) >= 0) {
        printf("Perfect match found; \"%s\" is in database\n", s);
        return;
    }

    int *scores = malloc((size_t)DB_SIZE * sizeof *scores);
    if (!scores) { perror("malloc"); return; }

    for (int i = 0; i < DB_SIZE; ++i) {
        scores[i] = levenshtein(s, DATABASE[i], false);
    }

    printf("No perfect match found; \"%s\" is not in database\n", s);
    printf("Best matches:\n");
    printf("Distance\tString\n");

    // find top 3 smallest distances
    int top = DB_SIZE < 3 ? DB_SIZE : 3;
    for (int k = 0; k < top; ++k) {
        int best_i = -1;
        for (int i = 0; i < DB_SIZE; ++i) {
            if (scores[i] >= 0 && (best_i < 0 || scores[i] < scores[best_i])) best_i = i;
        }
        if (best_i < 0) break;
        printf("%d\t\t%s\n", scores[best_i], DATABASE[best_i]);
        scores[best_i] = -1; // mark used
    }
    free(scores);
}

/* Function cmd_size: print number of items in database
*  prints number of items
*/
static void cmd_size(void) {
    printf("%d items in database\n", DB_SIZE);
}

/* Main program
*/
int main(void) {
    printf("Welcome to DNA Matcher v0.2\n");
    DB_SIZE = 0;
    memset(DATABASE, 0, sizeof(DATABASE));
    char line[MAX_INPUT_SIZE];

    while (1) {
        printf("console> ");
        if (!fgets(line, sizeof line, stdin)) break;
        // strip newline
        size_t L = strlen(line);
        if (L && line[L-1] == '\n') line[L-1] = '\0';
        char *cmd = strtok(line, " \t");

        if (!cmd) continue;

        if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) break;

        if (strcmp(cmd, "help") == 0) { cmd_help(); continue; }

        if (strcmp(cmd, "list") == 0) { cmd_list(); continue; }

        if (strcmp(cmd, "add") == 0) {
            char *arg = strtok(NULL, "\n");
            if (!arg) { printf("usage: add <string>\n"); continue; }
            // trim leading spaces
            while (*arg && isspace((unsigned char)*arg)) arg++;
            cmd_add(arg);
            continue;
        }
        
        if (strcmp(cmd, "compare") == 0) {
            char *arg1 = strtok(NULL, " \n");
            char *arg2 = strtok(NULL, " \n");
            if (!arg1 || !arg2) { 
                printf("usage: compare <string1> <string2>\n"); 
                continue; 
            }
            cmd_compare(arg1, arg2);
            continue;
        }

        if (strcmp(cmd, "delete") == 0 || strcmp(cmd, "remove") == 0) {
            char *arg = strtok(NULL, " \n");
            if (!arg) { 
                printf("usage: delete <string>\n"); 
                continue; 
            }
            cmd_delete(arg);
            continue;
        }

        if (strcmp(cmd, "read") == 0) {
            char *arg = strtok(NULL, " \n");
            if (!arg) { printf("usage: read <file>\n"); continue; }
            cmd_read(arg);
            continue;
        }
        if (strcmp(cmd, "retrieve") == 0) {
            char *arg = strtok(NULL, " \n");
            if (!arg) { printf("usage: retrieve <string>\n"); continue; }
            cmd_retrieve(arg);
            continue;
        }
        if (strcmp(cmd, "size") == 0) {
            cmd_size();
            continue;
        }

        printf("Unknown command '%s' (type 'help')\n", cmd);
    }

    // cleanup
    for (int i = 0; i < DB_SIZE; ++i) free(DATABASE[i]);
    return 0;
}
