/* hash functions for words: too_simple, fnv1a, djb2
 */
#include "hash_func.h"

/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long) *str;
}

/*
 * FNV-1a hash function implementation.
 * Reference: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */
unsigned long fnv1a_hash(const unsigned char *str) {
    unsigned long hash = 14695981039346656037UL;    // FNV offset basis

    while (*str) {
        hash ^= (unsigned long)(unsigned char)*str;
        hash *= 1099511628211UL;                    // FNV prime
        str++;
    }

    return hash;
}

/* djb2 hash function by Dan Bernstein
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long djb2_hash(const unsigned char *str) {
    unsigned long hash = 5381;

    while (*str) {
        hash = hash * 33 + *str;
        str++;
    }

    return hash;
}
