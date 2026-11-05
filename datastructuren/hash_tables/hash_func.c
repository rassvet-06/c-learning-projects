#include "hash_func.h"

/* 
 * This function is used by testing — do not remove it.
 * It simply returns the ASCII value of the first character.
 */
unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long)*str;
}

/* 
 * FNV-1a hash function implementation.
 * Reference: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 */
unsigned long fnv1a_hash(const unsigned char *str)
{
    unsigned long hash = 14695981039346656037UL;    // FNV offset basis

    while (*str) {
        hash ^= (unsigned long)(unsigned char)*str;
        hash *= 1099511628211UL;                    // FNV prime
        str++;
    }

    return hash;
}
