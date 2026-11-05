unsigned long hash_too_simple(const unsigned char *str);

/* 
 * FNV-1a hash function
 * Source: https://github.com/benhoyt/ht/blob/master/ht.h
 */
unsigned long fnv1a_hash(const unsigned char *str);

/* 
 * You may add more hash functions here if needed.
 * Make sure to implement them inside hash_func.c
 */

